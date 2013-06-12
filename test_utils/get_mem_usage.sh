#!/bin/sh -

k=1
[ "$1" = "-b" ]  && k=1 && shift
[ "$1" = "-kb" ] && k=1024 && shift
[ "$1" = "-mb" ] && k=1048576 && shift

if [ ! -x `which $1` ]; then
    echo "Cannot run: $1"
    exit 1
fi

rm -f massif.out*
valgrind --tool=massif $@ >/dev/null 2>&1
memusg=`ms_print massif.out.* | egrep '^ *([0-9,]+ +){3}' | sort -u -n -k 3 | tail -1 | sed 's/,//g' | awk '{print $3}'`
expr $memusg / $k
rm -f massif.out*

exit 0
