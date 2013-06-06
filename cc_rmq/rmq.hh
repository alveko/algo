#ifndef RMQ_HH
#define RMQ_HH

#include <cstddef>
#include <vector>
#include <stdint.h>  // <cstdint> uint32_t

#ifdef DEBUG
#include<stdio.h>
#define TDEBUG(...) printf(__VA_ARGS__)
#else
#define TDEBUG(...)
#endif

// naive rmq
size_t rmq_naive(const std::vector<int> &v, size_t lft, size_t rgt);

// sparse table
void   rmq_sparse_table_build(const std::vector<int> &v,
                              std::vector< std::vector<size_t> > &st);

size_t rmq_sparse_table_query(const std::vector<int> &v,
                              std::vector< std::vector<size_t> > &st,
                              size_t lft, size_t rgt);

// segment tree
size_t rmq_segment_tree_build(const std::vector<int> &v, std::vector<size_t> &st,
                              size_t node = 0, size_t lft = 0, size_t rgt = 0);

size_t rmq_segment_tree_query(const std::vector<int> &v,
                              const std::vector<size_t> &st,
                              size_t lft, size_t rgt,
                              size_t node = 0, size_t lftx = 0, size_t rgtx = 0);


uint8_t log2(uint32_t n);
uint8_t log2ceil(uint32_t n);

#endif
