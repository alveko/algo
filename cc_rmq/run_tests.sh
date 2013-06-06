#!/bin/sh -

rmq="./rmq"
if [ ! -x $rmq ]; then
    exit 1;
fi

out_pre="out_pre"
out_mem="out_mem"
out_rmq="out_rmq"
rm -f $out_pre $out_mem $out_rmq

size_min=5000000
size_inc=5000000
size_max=40000000

get_run_time()
{
    cmd=$@
    /usr/bin/time -f %e $cmd 2>&1 1>/dev/null
}

get_mem_usage()
{
    rm -f massif.out*
    valgrind --tool=massif $@ >/dev/null 2>&1
    memusg=`ms_print massif.out.* | egrep '^ *([0-9,]+ +){3}' | sort -u -n -k 3 | tail -1 | sed 's/,//g' | awk '{print $3}'`
    expr $memusg / \( 1024 \* 1024 \)
    rm -f massif.out*
}

test_precomp_time()
{
    # -------------------------------------------------------
    echo "\nTesting init time: sparse table and segment tree"
    
    for size in `seq $size_min $size_inc $size_max`; do
        
        cmd="$rmq --rmq sparsetable --size $size --q-num 0"
        echo $cmd
        t1=`get_run_time $cmd`
        sleep 1
        
        cmd="$rmq --rmq segmenttree --size $size --q-num 0"
        echo $cmd
        t2=`get_run_time $cmd`
        sleep 1
        
        out=`printf "%-10d %-7s %-7s\n" $size $t1 $t2`
        echo "$out"
        echo $out >> $out_pre
    done
}

test_mem_usage()
{
    # -----------------------------------------------------------
    echo "\nTesting memory usage : sparse table and segment tree"

    for size in `seq $size_min $size_inc $size_max`; do
        
        cmd="$rmq --rmq sparsetable --size $size --q-num 0"
        echo $cmd
        r1=`get_mem_usage $cmd`
        sleep 1
        
        cmd="$rmq --rmq segmenttree --size $size --q-num 0"
        echo $cmd
        r2=`get_mem_usage $cmd`
        sleep 1
        
        out=`printf "%-10d %-7s %-7s\n" $size $r1 $r2`
        echo "$out"
        echo $out >> $out_mem
    done
}

test_rmq()
{
    # -------------------------------------------------------------
    echo "\nTesting RMQ time: naive, sparse table and segment tree"

    size=1000000

    for n in `seq    1000    1000     10000` \
             `seq 1000000 1000000  10000000`; do

        cmd1="$rmq --rmq sparsetable --size $size --q-num $n"
        cmd2="$rmq --rmq segmenttree --size $size --q-num $n"
        cmd3="$rmq --rmq naive       --size $size --q-num $n"
        
        echo "$cmd1"
        t1=`get_run_time $cmd1`
        sleep 1
        
        echo "$cmd2"
        t2=`get_run_time $cmd2`
        sleep 1

        t3=""
        if [ $n -le 10000 ]; then
            echo "$cmd3"
            t3=`get_run_time $cmd3`
            sleep 1
        fi
        
        out=`printf "%-10d %-7s %-7s %-7s\n" $n $t1 $t2 $t3`
        echo "$out"
        echo $out >> $out_rmq
        
    done
}

test_precomp_time
test_mem_usage
test_rmq

exit 0

