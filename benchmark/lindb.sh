#!/bin/bash
# simple benchmarking program
#set -o xtrace
#set -o errexit

TO_RUN="benchmark/benchmark.nexe"

date > results
for prog in $TO_RUN
do
    lind $prog | grep '>>' | cut -c3- >> results

done


