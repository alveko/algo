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
              xlabel  = "Size of the input sequence",
              y1label = "Time (sec)",
              y2label = "Memory (Mb)",
              title1  = "time",
              title2  = "memory",
              file1   = "out_time_dp",
              file2   = "out_memo_dp",
              y2rmin  = "*",
              y2rmax  = "10",
              y2factor = "0.001")
    test_utils.gnuplot_1x2y(**gp)

    gp = dict(outpng  = "plot_nlogn.png",
              title   = "LIS - Algorithm O(N log(N))^{}",
              xlabel  = "Size of the input sequence",
              y1label = "Time (sec)",
              y2label = "Memory (Mb)",
              title1  = "time",
              title2  = "memory",
              file1   = "out_time_nlogn",
              file2   = "out_memo_nlogn",
              y1rmin  = "*",
              y1rmax  = "10")
    test_utils.gnuplot_1x2y(**gp)

def main():
    run_tests()
    run_gnuplot()

if __name__ == "__main__":
    main()
