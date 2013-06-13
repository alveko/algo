/*******************************************************************************
 * File   : Math.hpp
 * Brief  : Simple math functions
 *
 * Author : Alexander Korobeynikov (alexander.korobeynikov@gmail.com)
 *
 *******************************************************************************
 */
#ifndef ALGO_MATH_HPP
#define ALGO_MATH_HPP

#include <stdint.h>  // <cstdint> uint32_t

namespace algo
{
    uint8_t log2(uint32_t n)
    {
        uint8_t lg2 = 0;
        while (n >>= 1)
            lg2++;
        return lg2;
    }
    
    uint8_t log2ceil(uint32_t n)
    {
        uint8_t lg2 = log2(n);
        return lg2 + !!(n ^ (1<<lg2));
    }
}

#endif
