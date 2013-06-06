#include <cassert>
#include "rmq.hh"

// time = O(nlogn), memory = O(nlogn)

void rmq_sparse_table_build(const std::vector<int> &v,
                            std::vector< std::vector<size_t> > &st)
{
    size_t n = v.size();
    size_t logn = log2(n);
    
    st.clear();
    st.resize(logn + 1);

    // create a sparse table (st) for the input array (v)
    // st[j][i] represents a range in the input array (v),
    // the range begins at i and has a size of 2^j (1<<j)
    // st[j][i] is the index of the minimum element in the range

    // st[][] is filled using dynamic programming approach

    st[0].resize(n);
    for (size_t i = 0; i < n; i++) {
        st[0][i] = i;
    }

    for (size_t j = 1; j < st.size(); j++) {
        
        assert(j < logn + 1);
        st[j].resize(n - (1<<j) + 1);
        
        for (size_t i = 0; i < st[j].size(); i++) {

            // range size 2^j doubles at every iteration
            // => can be reduced to two halves of previous iteration
            assert(i+(1<<(j-1)) < st[j-1].size());
            size_t half1 = st[j-1][i];
            size_t half2 = st[j-1][i+(1<<(j-1))];
            
            st[j][i] = (v[half1] <= v[half2]) ? half1 : half2;

            TDEBUG("j = %2zu, i = %2zu, rgt = %2zu, st = %2zu, v = %d\n",
                   j, i, i+(1<<j)-1, st[j][i], v[st[j][i]]);
        }
    }
}

size_t rmq_sparse_table_query(const std::vector<int> &v,
                              std::vector< std::vector<size_t> > &st,
                              size_t lft, size_t rgt)
{
    if (lft < 0 || lft >= v.size() ||
        rgt < 0 || rgt >= v.size())
        return 0;

    // find the biggest subrange size covered by the rmq
    size_t k = log2((rgt - lft) + 1);

    // two subranges covering the rmq
    size_t subrange1 = st[k][lft];
    size_t subrange2 = st[k][rgt - (1<<k) + 1];

    return (v[subrange1] <= v[subrange2]) ? subrange1 : subrange2;
}
