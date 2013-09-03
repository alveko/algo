/*******************************************************************************
 * File   : algo.hpp
 * Brief  : Micellaneous auxiliary macros and functions
 *
 * Author : Alexander Korobeynikov (alexander.korobeynikov@gmail.com)
 *
 *******************************************************************************
 */
#ifndef ALGO_HPP
#define ALGO_HPP

#ifdef DEBUG
#include <boost/format.hpp>
#define TDEBUG(x) std::cout << boost::format x ;
#else
#define TDEBUG(...)
#endif

#endif
