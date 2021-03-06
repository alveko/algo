/*******************************************************************************
 * File   : array_2d_transpose.hpp
 * Brief  : Transpose 2D array inplace
 *
 * Author : Alexander Korobeynikov (alexander.korobeynikov@gmail.com)
 *
 *******************************************************************************
 */
#ifndef ARRAY_2D_TRANSPOSE_HPP
#define ARRAY_2D_TRANSPOSE_HPP

#include <cassert>

namespace algo
{
    // n rows
    // m columns

    template<typename RandomIterator, typename DT =
             typename std::iterator_traits<RandomIterator>::difference_type>
    void array_2d_transpose_v1(RandomIterator arr, DT n, DT m)
    {
        if (n <= 1 || m <= 1) {
            return;
        }

        std::vector<bool> visited(n*m, false);
        const DT mn1 = m * n - 1;
        DT cycle = 0;

        while (++cycle != mn1 - 1) {

            if (!visited[cycle]) {

                DT i = cycle;
                do  {
                    i = (n * i) % mn1;
                    std::swap(arr[i], arr[cycle]);
                    visited[i] = true;
                } while (i != cycle);
            }
        }
    }

    template<typename RandomIterator, typename DT =
             typename std::iterator_traits<RandomIterator>::difference_type>
    void array_2d_transpose(RandomIterator arr, DT n, DT m)
    {
        if (n <= 1 || m <= 1) {
            return;
        }

        const DT mn1 = m * n - 1;
        DT cycle = 0, cycle_len, i;

        while (++cycle != mn1 - 1) {

            cycle_len = 0;
            i = cycle;
            do  {
                i = (n * i) % mn1;
                cycle_len++;
            } while (i > cycle);

            if (cycle_len == 1 || i != cycle)
                continue;

            i = cycle;
            do  {
                i = (n * i) % mn1;
                std::swap(arr[i], arr[cycle]);
            } while (i != cycle);
        }
    }

}

#endif
