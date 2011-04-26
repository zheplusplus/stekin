#!/bin/sh

echo "sample-test:"

verify() {
    if ./stkn.sh samples/$1.stkn tmp.out && ./tmp.out | diff samples/$1.expected - ;
    then
        echo $1 "pass."
    else
        echo $1 "FAILED!"
    fi
}

verify empty
verify write
verify fib
verify nest-func
verify return-void
verify pair
verify sqrt
verify find-root
verify fixed-point
verify vector-multi
verify big-literals
