#!/bin/sh

verify() {
    if ./stkn.sh -cm samples/$1.stkn tmp.out && ./tmp.out | diff samples/$1.expected - ;
    then
        echo $1 "pass."
    else
        echo $1 "FAILED!"
    fi
}

if [ $# == 1 ];
then
    verify $1
    exit
fi

echo "sample-test:"

verify empty
verify write
verify latency-ref
verify fib
verify nest-func
verify return-void
verify pair
verify sqrt
verify find-root
verify fixed-point
verify vector-multi
verify big-literals
