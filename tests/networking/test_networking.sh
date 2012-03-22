#! /bin/bash
set -o errexit



make clean all

lind ./nettest-server &
sleep 5
lind ./nettest-client
wait

killall python2.6

rm -rf lind.metadata

