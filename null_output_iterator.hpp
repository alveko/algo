/*******************************************************************************
 * File   : null_output_iterator.hpp
 * Brief  : Dummy (null) output iterator.
 *          It can be used as a stub iterator for template parameters.
 *
 * Author : Alexander Korobeynikov (alexander.korobeynikov@gmail.com)
 *
 *******************************************************************************
 */
#ifndef NULL_OUTPUT_ITERATOR_HPP
#define NULL_OUTPUT_ITERATOR_HPP

#include <iterator> 

namespace algo
{
    struct null_output_iterator : 
        std::iterator< std::output_iterator_tag,
                       null_output_iterator >
    {
        // empty assignment
        template<typename T>
        void operator=(T const&) { }
        
        // dummy operators
        null_output_iterator & operator++()    { return *this; }
        null_output_iterator   operator++(int) { return *this; }
        null_output_iterator & operator* ()    { return *this; }
    };    
}

#endif
