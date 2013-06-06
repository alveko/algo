#include "rmq.hh"

// time = O(n), memory = O(1)

size_t rmq_naive(const std::vector<int> &v, size_t lft, size_t rgt)
{
    if (lft < 0 || lft >= v.size() ||
        rgt < 0 || rgt >= v.size())
        return 0;

    size_t rmq = lft;
    for (size_t i = lft; i <= rgt; i++) {
        if (v[i] < v[rmq]) {
            rmq = i;
        }
    }

    return rmq;
}
