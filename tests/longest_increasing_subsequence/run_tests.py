#!/usr/bin/python

import sys
sys.path.append('../test_utils')
import test_utils

def tc01_nlogn():
    n = 10**8
    seq = [n/10*i for i in range(1, 11)]
    cmd = "./lis --lis nlogn --size $x"

    test_utils.run_seq_time("Testing run time:",
                            seq, cmd, "out_time_nlogn", 5)

    test_utils.run_seq_memo("Testing memory usage:",
                            seq, cmd, "out_memo_nlogn", 1, "mb")

def tc02_dp():
    n = 10**5
    seq = [n/10*i for i in range(1, 11)]
    cmd = "./lis --lis dp --size $x"

    test_utils.run_seq_time("Testing run time:",
                            seq, cmd, "out_time_dp", 1)

    test_utils.run_seq_memo("Testing memory usage:",
                            seq, cmd, "out_memo_dp", 1, "kb")

def run_tests():
    tc01_nlogn()
    tc02_dp()

def run_gnuplot():
    gp = dict(outpng  = "plot_dp.png",
              title   = "LIS - Dynamic Programming O(N^{2})",
              labelx  = "Size of the input sequence",
              labely1 = "Time (sec)",
              labely2 = "Memory (Mb)",
              title1  = "time",
              title2  = "memory",
              file1   = "out_time_dp",
              file2   = "out_memo_dp",
              rminy2  = "*",
              rmaxy2  = "10",
              factory2 = "0.001")
    test_utils.gnuplot_x1y2p2(gp)

    gp = dict(outpng  = "plot_nlogn.png",
              title   = "LIS - Algorithm O(N log(N))^{}",
              labelx  = "Size of the input sequence",
              labely1 = "Time (sec)",
              labely2 = "Memory (Mb)",
              title1  = "time",
              title2  = "memory",
              file1   = "out_time_nlogn",
              file2   = "out_memo_nlogn",
              rminy1  = "*",
              rmaxy1  = "10")
    test_utils.gnuplot_x1y2p2(gp)

def main():
    run_tests()
    run_gnuplot()

if __name__ == "__main__":
    main()
