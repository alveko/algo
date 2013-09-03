#!/bin/sh -

if [ ! -x `which $1` ]; then
    echo "Cannot run: $1"
    exit 1
fi

cmd=$@
/usr/bin/time -f %e $cmd 2>&1 1>/dev/null

exit 0
