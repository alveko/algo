/*******************************************************************************
 * File   : longest_increasing_subsequence.hpp
 * Brief  : Algorithms to find a longest increasing subsequence
 *
 * Author : Alexander Korobeynikov (alexander.korobeynikov@gmail.com)
 *
 *******************************************************************************
 */
#ifndef LONGEST_INCREASING_SUBSEQUENCE_HPP
#define LONGEST_INCREASING_SUBSEQUENCE_HPP

#include <iterator> 
#include <vector>
#include <algorithm> // std::max()

#include "null_output_iterator.hpp"

namespace algo
{

    // @brief  Compute longest increasing subsequence in the input sequence
    //
    // @func   longest_increasing_subsequence
    // @time   O(N logN)
    // @space  O(N)
    //
    // @param [in]  begin - random iterator to the start of the sequence
    // @param [in]  end   - random iterator to the end of the sequence
    // @param [out] out   - optional output iterator to write the
    //                      LIS indecies to
    // @param [in]  comp  - opional comparator, by default std::less
    // @return value      - length of longest increasing subsequence
    //
    // @example
    // std::vector<int> seq = { 1, 0, 2, 0, 3 };
    // std::vector<size_t> lis;
    // size_t lislen = longest_increasing_subsequence(seq.begin(), seq.end(),
    //                                                std::back_inserter(lis));
    
    template <typename RandomIterator,
              typename OutputIterator = null_output_iterator,
              typename Comparator =
              std::less< typename std::iterator_traits<RandomIterator>::value_type> >
    
    typename std::iterator_traits<RandomIterator>::difference_type
    longest_increasing_subsequence(RandomIterator begin,
                                   RandomIterator end,
                                   OutputIterator out = OutputIterator(),
                                   Comparator comp = Comparator())
    {
        typedef typename std::iterator_traits<RandomIterator>::difference_type DT;
        const DT undef = (DT)-1;
        const DT seqlen = end - begin;
        
        std::vector<DT> tails(seqlen + 1, undef);
        std::vector<DT> prevs(seqlen, undef);
        
        DT maxlen = 0;
        for (DT i = 0; i < seqlen; ++i) {
            
            // binary search for a tail less than the current element
            DT j = 0, lo = 1, hi = maxlen;
            while (lo <= hi) {
                DT m = lo + (hi - lo) / 2;
                if (comp(begin[tails[m]], begin[i])) {
                    j = m;
                    lo = m + 1;
                } else {
                    hi = m - 1;
                }
            }

            prevs[i] = tails[j];
            if (j == maxlen || comp(begin[i], begin[tails[j+1]])) {
                tails[j+1] = i;
                maxlen = std::max(maxlen, j + 1);
            }
        }

        // backtrack and store the result
        if (typeid(out) != typeid(null_output_iterator)){
            std::vector<DT> lis(maxlen, 0);
            DT n = maxlen;
            DT i = tails[maxlen];
            while (i != undef && n > 0) {
                lis[--n] = i;
                i = prevs[i];
            }
            std::copy(lis.begin(), lis.end(), out);
        }
        
        return maxlen;
    }

    // @brief  Compute longest increasing subsequence in the input sequence
    //         Dynamic programming approach.
    //
    // @func   longest_increasing_subsequence_dp
    // @time   O(N^2)
    // @space  O(N)
    //
    // @param [in]  begin - random iterator to the start of the sequence
    // @param [in]  end   - random iterator to the end of the sequence
    // @param [out] out   - optional output iterator to write the
    //                      LIS indecies to
    // @param [in]  comp  - opional comparator, by default std::less
    // @return value      - length of longest increasing subsequence
    //
    // @example
    // std::vector<int> seq = { 1, 0, 2, 0, 3 };
    // std::vector<size_t> lis;
    // size_t lislen = longest_increasing_subsequence_dp(seq.begin(), seq.end(),
    //                                                   std::back_inserter(lis));

    template <typename RandomIterator,
              typename OutputIterator = null_output_iterator,
              typename Comparator =
              std::less< typename std::iterator_traits<RandomIterator>::value_type> >
    
    typename std::iterator_traits<RandomIterator>::difference_type
    longest_increasing_subsequence_dp(RandomIterator begin,
                                      RandomIterator end,
                                      OutputIterator out = OutputIterator(),
                                      Comparator comp = Comparator())
    {
        typedef typename std::iterator_traits<RandomIterator>::difference_type DT;
        const DT undef = (DT)-1;
        const DT seqlen = end - begin;
            
        // at start, lislen[i] = 1 for all i
        // meaning longest known increasing subsequence consists of just one
        // element, the element at index i
        std::vector<DT> lislen(seqlen, 1);
        std::vector<DT> prevs(seqlen, undef);
        DT maxlen = (!!(seqlen)), maxlen_ind = 0;

        // for each index i in the input sequence, starting from the second
        for (DT i = 1; i < seqlen; ++i) {

            // for each index j, which is before index i in the input sequence
            for (DT j = 0; j < i; ++j) {

                // check if:
                // 1. length of longest so far increasing subsequnce ending at j
                //    is at least as long as longest so far incr. subseq ending at i
                // 2. element at j is less than element at i, meaning that
                //    the increasing subsequence ending at i can be extended by
                //    the increasing subsequence ending at j + element at i
                if (lislen[j] >= lislen[i] &&
                    comp(begin[j], begin[i])) {
                    
                    lislen[i] = lislen[j] + 1;

                    if (maxlen < lislen[i]) {
                        maxlen = lislen[i];
                        maxlen_ind = i;
                    }

                    // remember the previous index j to backtrack later
                    prevs[i] = j;
                }
            }
        }

        // backtrack and store the result
        if (typeid(out) != typeid(null_output_iterator)){
            std::vector<DT> lis(maxlen, 0);
            DT n = maxlen;
            DT i = maxlen_ind;
            while (i != undef && n > 0) {
                lis[--n] = i;
                i = prevs[i];
            }
            std::copy(lis.begin(), lis.end(), out);
        }

        return maxlen;
    }
}

#endif
