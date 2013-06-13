#!/usr/bin/python

import os
import time
import subprocess

testdir = os.path.dirname(os.path.realpath(__file__))

def run_seq(msg, seq, cmd, file = "out", sleep = 0, cmdprefix = ""):

    print "\n*****", msg

    i = 0
    f = open(file, "w")
    for x in seq:
        i = i + 1
        run = cmdprefix + cmd.replace("$x", str(x))
        print ("\n%02d > %s") % (i, run)

        # run command
        out = subprocess.check_output(run + "; exit 0",
                                      stderr = subprocess.STDOUT,
                                      shell = True)
        if out:
            print out.rstrip('\n')
        f.write(("%s %s") % (x, out))

        # sleep between commands
        time.sleep(sleep)
        
    f.close()
    
def run_seq_time(*args):
    run_seq(*args, cmdprefix = testdir + "/get_run_time.sh ")
    
def run_seq_memo(*args):
    run_seq(*args[:-1], cmdprefix = testdir +
            "/get_mem_usage.sh -" + args[-1] + " ")

def gnuplot_1x2y(**args):
    ### open template and create gnupot process
    f = open(testdir + "/gnuplot.in.1x2y", "r")
    process = subprocess.Popen('gnuplot', stdin=subprocess.PIPE)

    ### send parameters to gnuplot
    for k, v in args.iteritems():
        process.stdin.write(("%s = \"%s\"\n") % (k, v))

    ### send template to gnuplot
    for line in f:
        process.stdin.write(line)
        
    ### run and close
    process.communicate()
    f.close()
    
def main():
    output = run_seq("Main", [1, 2, 3], "echo $x")
    print "\noutput:\n", ''.join(output),

if __name__ == "__main__":
    main()