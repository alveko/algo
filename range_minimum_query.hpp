/// ****************************************************************************
///
/// @file   : range_minimum_query.hpp
/// @brief  : Algorithms to perfrom Range Minimum Queries
///
/// @author : Alexander Korobeynikov (alexander.korobeynikov@gmail.com)
///
/// Contents:
/// 1. Naive linear RMQ computation
/// 2. Generic RMQ with sparse table
/// 3. Generic RMQ with segment tree
/// 4. TODO: +-1 RMQ with sparse table
///
/// ****************************************************************************
#ifndef ALGO_RANGE_MINIMUM_QUERY_HPP
#define ALGO_RANGE_MINIMUM_QUERY_HPP

#include <iterator>
#include <vector>
#include <cassert>
#include <algorithm>  // std::max()
#include "math.hpp"   // log2(), log2ceil()

namespace algo
{

/// "True" C++ STL-like code would use the iterator traits:
///     typename std::iterator_traits<RandomIterator>::difference_type;
/// instead of size_type (or size_t) everywhere below. But doing so would
/// sacrifice readability of the code which I'm not ready for right now.
using size_type = size_t;

/// ****************************************************************************
/// *** Naive linear RMQ computation

/// ----------------------------------------------------------------------------
/// @brief Naive linear search for min/max element in the given range.
///        Time O(N) time. Memory O(1).
///
/// @param[in]  begin,end   random iterator to the start,end of the input array
/// @param[in]  left,right  left,right index of RMQ
/// @param[in]  comp        opional comparator, by default std::less
/// @return                 RMQ result (index of the min/max element)
template <typename RandomIterator,
          typename Comparator = std::less<
              typename std::iterator_traits<RandomIterator>::value_type> >
size_type rmq_naive_linear(RandomIterator begin, RandomIterator /*end*/,
                           size_type left, size_type right,
                           Comparator comp = Comparator())
{
    size_type rmq = left;
    for (size_type i = left; i <= right; i++) {
        if (comp(begin[i], begin[rmq])) {
            rmq = i;
        }
    }
    return rmq;
}

/// ****************************************************************************
/// *** Generic RMQ with sparse table

/// sparse_table[j][i] represents a range in the input array,
/// which begins at i and has a size of 2^j (1<<j) elements
/// sparse_table[j][i] is the index of the minimum element in the range

using sparse_table = std::vector<std::vector<size_type> >;

/// ----------------------------------------------------------------------------
/// @brief Builds a sparse table for generic RMQ.
///        Time O(N logN). Memory O(N logN).
///
/// @param[in]  begin,end  random iterator to the start,end of the input array
/// @param[in]  comp       opional comparator, by default std::less
/// @return                sparse_table
template <typename RandomIterator,
          typename Comparator = std::less<
              typename std::iterator_traits<RandomIterator>::value_type> >
sparse_table rmq_sparse_table_build(RandomIterator begin, RandomIterator end,
                                    Comparator comp = Comparator())
{
    size_type n = std::distance(begin, end);
    size_type logn = algo::log2(n);

    // sparse table is filled using bottom-up dynamic programming approach
    sparse_table st;
    st.resize(logn + 1);

    st[0].resize(n);
    for (size_type i = 0; i < n; i++) {
        st[0][i] = i;
    }

    for (size_type j = 1; j < st.size(); j++) {
        assert(j < logn + 1);
        st[j].resize(n - (1 << j) + 1);

        for (size_type i = 0; i < st[j].size(); i++) {

            // range size 2^j doubles at every iteration
            // => can be reduced to two halves of previous iteration
            assert(i + (1 << (j - 1)) < st[j - 1].size());
            size_type half1 = st[j - 1][i];
            size_type half2 = st[j - 1][i + (1 << (j - 1))];

            st[j][i] = (comp(begin[half2], begin[half1])) ? half2 : half1;
        }
    }

    return st;
}

/// ----------------------------------------------------------------------------
/// @brief RMQ with a sparse table.
///        Time O(1).
///
/// @param[in]  st          sparse table built with rmq_sparse_table_build()
/// @param[in]  begin,end   random iterator to the start,end of the input array
/// @param[in]  left,right  left,right index of RMQ
/// @param[in]  comp        opional comparator, by default std::less
/// @return                 sparse_table
template <typename RandomIterator,
          typename Comparator = std::less<
              typename std::iterator_traits<RandomIterator>::value_type> >
size_type rmq_sparse_table_query(const sparse_table &st, RandomIterator begin,
                                 RandomIterator end, size_t left, size_t right,
                                 Comparator comp = Comparator())
{
    // find the biggest subrange size covered by the rmq
    size_type k = algo::log2((right - left) + 1);

    // two subranges covering the rmq
    size_type subrange1 = st[k][left];
    size_type subrange2 = st[k][right - (1 << k) + 1];

    return comp(begin[subrange2], begin[subrange1]) ? subrange2 : subrange1;
}

/// ****************************************************************************
/// *** Generic RMQ with segment tree

/// Segment tree is a complete binary tree with heap-like stucture in
/// regards to the tree indecies in stored in a continuous array.
/// The root is at index 0. Children of a node i have indecies 2*i+1 and 2*i+2.
///
/// Every node in the tree represents a segment (subrange) of the input array
/// and is associated with an index of an min/max element in that subrange.
///
/// Thus, the root node represents the entire array [0,N-1] and is assosiated
/// with an index of min/max element in the entire input array.
///
/// A node at index (i) represents the subrange [left, right], then:
/// the first child at index (2*i+1) represents subrange [left, middle]
/// the second child (2*i+2) represents subrange [middle+1, right].
/// The leaves represent single elements of the input array (N leaves).

using segment_tree = std::vector<size_type>;

/// ----------------------------------------------------------------------------
/// @brief Builds a segment tree for generic RMQ (recursively!)
///        Time O(N). Space O(N).
///
/// @param[in]  begin,end  random iterator to the begin,end of the input array
/// @param[in]  st         segment tree
/// @param[in]  node       [recursive] current node of the segment tree
/// @param[in]  left,right [recursive] left,right index of the current subrange
/// @param[in]  comp       opional comparator, by default std::less
/// @return                index of min/max element of the subrange
template <typename RandomIterator,
          typename Comparator = std::less<
              typename std::iterator_traits<RandomIterator>::value_type> >
size_type rmq_segment_tree_build(RandomIterator begin, RandomIterator end,
                                 segment_tree &st, size_type node = 0,
                                 size_t left = 0, size_t right = 0,
                                 Comparator comp = Comparator())
{
    const size_type undef = (size_type) - 1;

    // early check for end of recursion
    if (left > right)
        return undef;

    size_type n = std::distance(begin, end);

    if (node == 0) {
        // it's first call => allocate segment tree
        st.clear();

        // segment tree size (n is the number of elements in the input array)
        // ->   (log2ceil(n)) is the level that can hold all distinct elements
        // -> 2^(log2ceil(n)) is the number of elements at that level
        // -> 2^(log2ceil(n)+1)-1 is the total number of elements in the tree
        st.resize((1 << (algo::log2ceil(n) + 1)) - 1);

        left = 0;
        right = n - 1;
    }

    assert(node >= 0 && node < st.size());
    size_type smin = undef;

    if (left < right) {
        // recurse on two halves of the segment (left and right subtrees)
        size_type mdl = left + (right - left) / 2;
        size_type sub1 =
            rmq_segment_tree_build(begin, end, st, 2 * node + 1, left, mdl);
        size_type sub2 = rmq_segment_tree_build(begin, end, st, 2 * node + 2,
                                                mdl + 1, right);
        smin = comp(begin[sub2], begin[sub1]) ? sub2 : sub1;
    } else {
        // left == right
        smin = left;
    }

    return st[node] = smin;
}

/// ----------------------------------------------------------------------------
/// @brief RMQ with a segment tree.
///        Time O(NlogN).
///
/// @param[in] begin,end     random iterator to the begin,end of the input array
/// @param[in] st            segment tree
/// @param[in] left,right    left,right index of the RMQ
/// @param[in] node          [recursive] current node of the segment tree
/// @param[in] leftx,rightx  [recursive] left,right index of the current subrange
/// @param[in] comp          opional comparator, by default std::less
/// @return                  index of min/max element of the subrange
template <typename RandomIterator,
          typename Comparator = std::less<
              typename std::iterator_traits<RandomIterator>::value_type> >
size_type rmq_segment_tree_query(RandomIterator begin, RandomIterator end,
                                 const segment_tree &st, size_t left,
                                 size_t right, size_t node = 0,
                                 size_t leftx = 0, size_t rightx = 0,
                                 Comparator comp = Comparator())
{
    const size_type undef = (size_type) - 1;
    size_type n = std::distance(begin, end);

    // the first query call => start from the tree root
    if (node == 0) {
        leftx = 0;
        rightx = n - 1;
    }

    // early check for end of recursion
    if (rightx < left ||  // the current segment is out of the query segment
        leftx > right ||
        leftx > rightx ||  // bad current segment
        left > right)      // bad query segment
        return undef;

    assert(node >= 0 && node < st.size());

    // the current segment is included into the query segment
    if (left <= leftx && rightx <= right) {
        return st[node];
    }

    size_type mdlx = leftx + (rightx - leftx) / 2;
    size_type sub1 = rmq_segment_tree_query(begin, end, st, left, right,
                                            2 * node + 1, leftx, mdlx);
    size_type sub2 = rmq_segment_tree_query(begin, end, st, left, right,
                                            2 * node + 2, mdlx + 1, rightx);

    if (sub1 == undef)
        return sub2;

    if (sub2 == undef)
        return sub1;

    assert(sub1 >= 0 && sub1 < n);
    assert(sub2 >= 0 && sub2 < n);
    return comp(begin[sub2], begin[sub1]) ? sub2 : sub1;
}

} // namespace algo

#endif
