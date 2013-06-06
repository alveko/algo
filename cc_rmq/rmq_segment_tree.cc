#include <cassert>
#include "rmq.hh"

size_t rmq_segment_tree_build(const std::vector<int> &v, std::vector<size_t> &st,
                              size_t node /*= 0*/, size_t lft /*= 0*/, size_t rgt /*= 0*/)
{
    // early check for end of recursion
    if (lft > rgt)
        return (size_t)-1;

    if (node == 0) {
        // it's first call => allocate segment tree
        st.clear();

        // segment tree size (n is the number of elements in the input array v)
        // ->   (log2ceil(n)) is the level that can hold all distinct elements
        // -> 2^(log2ceil(n)) is the number of elements at that level
        // -> 2^(log2ceil(n)+1)-1 is the total number of elements in the tree
        st.resize((1<<(log2ceil(v.size())+1))-1);
        
        lft = 0;
        rgt = v.size() - 1;
        TDEBUG("segm.tree size = %2zu\n", st.size());
    }

    assert(node >=0 && node < st.size());
    size_t smin = (size_t)-1;

    if (lft < rgt) {
        // recurse on two halves of the segment (left and right subtrees)
        size_t mdl = lft + (rgt - lft)/2;
        size_t sub1 = rmq_segment_tree_build(v, st, 2*node + 1, lft, mdl);
        size_t sub2 = rmq_segment_tree_build(v, st, 2*node + 2, mdl + 1, rgt);
        smin = (v[sub1] < v[sub2]) ? sub1 : sub2;
    } else {
        // lft == rgt
        smin = lft;
    }

    TDEBUG("%02zu: [%2zu, %2zu] = %2zu\n", node, lft, rgt, smin);
    return st[node] = smin;
}

size_t rmq_segment_tree_query(const std::vector<int> &v,
                              const std::vector<size_t> &st,
                              size_t lft, size_t rgt,
                              size_t node /*= 0*/, size_t lftx /*= 0*/, size_t rgtx /*= 0*/)
{
    // the first query call => start from the tree root
    if (node == 0) {
        lftx = 0;
        rgtx = v.size() - 1;
    }

    // early check for end of recursion
    if (lftx > rgt || rgtx < lft || // the current segment is out of the query segment
        lft > rgt || lftx > rgtx)   // bad query segment or bad current segment
        return (size_t)-1;

    assert(node >=0 && node < st.size());
    
    // the current segment is included into the query segment
    if (lftx >= lft &&
        rgtx <= rgt) {
        return st[node];
    }
    
    size_t mdlx = lftx + (rgtx - lftx)/2;
    size_t sub1 = rmq_segment_tree_query(v, st, lft, rgt, 2*node + 1, lftx, mdlx);
    size_t sub2 = rmq_segment_tree_query(v, st, lft, rgt, 2*node + 2, mdlx + 1, rgtx);

    if (sub1 == (size_t)-1)
        return sub2;

    if (sub2 == (size_t)-1)
        return sub1;
    
    assert(sub1 >=0 && sub1 < v.size());
    assert(sub2 >=0 && sub2 < v.size());
    
    return (v[sub1] < v[sub2]) ? sub1 : sub2;
}
