#! /bin/bash


make clean all
lind ./nettest-server &
sleep 2
lind ./nettest-client
killall lind
