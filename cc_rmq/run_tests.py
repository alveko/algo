#!/usr/bin/python

import sys
sys.path.append('../test_utils')
import test_utils

def tc_pre_sparse_table():
    n = 30*(10**6)
    seq = [n/10*i for i in range(1, 11)]
    cmd = "./rmq --rmq sparsetable --size $x --q-num 0 "

    test_utils.run_seq_time("Testing run time:",
                            seq, cmd, "out_time_sparsetable_pre", 1)

    test_utils.run_seq_memo("Testing memory usage:",
                            seq, cmd, "out_memo_sparsetable_pre", 1, "mb")

def tc_pre_segment_tree():
    n = 30*(10**6)
    seq = [n/10*i for i in range(1, 11)]
    cmd = "./rmq --rmq segmenttree --size $x --q-num 0 "

    test_utils.run_seq_time("Testing run time:",
                            seq, cmd, "out_time_segmenttree_pre", 1)

    test_utils.run_seq_memo("Testing memory usage:",
                            seq, cmd, "out_memo_segmenttree_pre", 1, "mb")

def tc_rmq_naive():
    pass

def tc_rmq_sparse_table():
    n = 10*(10**6)
    seq = [n/10*i for i in range(1, 11)]
    cmd = "./rmq --rmq sparsetable --size 1000000 --q-num $x"

    test_utils.run_seq_time("Testing run time:",
                            seq, cmd, "out_time_sparsetable", 1)

def tc_rmq_segment_tree():
    n = 10*(10**6)
    seq = [n/10*i for i in range(1, 11)]
    cmd = "./rmq --rmq segmenttree --size 1000000 --q-num $x"

    test_utils.run_seq_time("Testing run time:",
                            seq, cmd, "out_time_segmenttree", 1)

def run_tests():
    tc_pre_sparse_table()
    tc_pre_segment_tree()
    tc_rmq_naive()
    tc_rmq_sparse_table()
    tc_rmq_segment_tree()

def run_gnuplot():
    gp = dict(outpng  = "plot_sparsetable_pre.png",
              title   = "RMQ - Sparse table precomputing",
              labelx  = "Size of the input array",
              labely1 = "Time (sec)",
              labely2 = "Memory (Mb)",
              title1  = "time",
              title2  = "memory",
              file1   = "out_time_sparsetable_pre",
              file2   = "out_memo_sparsetable_pre")
    test_utils.gnuplot_x1y2p2(gp)

    gp = dict(outpng  = "plot_segmenttree_pre.png",
              title   = "RMQ - Segment tree precomputing",
              labelx  = "Size of the input array",
              labely1 = "Time (sec)",
              labely2 = "Memory (Mb)",
              title1  = "time",
              title2  = "memory",
              file1   = "out_time_segmenttree_pre",
              file2   = "out_memo_segmenttree_pre")
    test_utils.gnuplot_x1y2p2(gp)

    gp = dict(outpng  = "plot_pre.png",
              title   = "RMQ - Sparse table and Segment tree precomputing",
              labelx  = "Size of the input array",
              labely1 = "Time (sec)",
              labely2 = "Memory (Mb)",
              title1  = "sparse table time",
              title2  = "sparse table memory",
              title3  = "segment tree time",
              title4  = "segment tree memory",
              file1   = "out_time_sparsetable_pre",
              file2   = "out_memo_sparsetable_pre",
              file3   = "out_time_segmenttree_pre",
              file4   = "out_memo_segmenttree_pre")
    test_utils.gnuplot_x1y2p4(gp)

    gp = dict(outpng  = "plot_rmq.png",
              title   = "RMQ - Sparse table and Segment tree",
              labelx  = "Number of RMQs (input array size = 10^{6})",
              labely1 = "Time (sec)",
              title1  = "sparse table time",
              title2  = "segment tree time",
              file1   = "out_time_sparsetable",
              file2   = "out_time_segmenttree")
    test_utils.gnuplot_x1y1p2(gp)

def main():
    run_tests()
    run_gnuplot()

if __name__ == "__main__":
    main()
