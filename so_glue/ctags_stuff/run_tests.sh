#! /bin/bash

set -o errexit

cd /home/lind/lind/misc/so_glue/network/src/
make clean all

cd /home/lind/lind/misc/so_glue/ctags_stuff/
cd test
make clean

cd ..
cd src
rm -rf ../output/*_mm*
./runme.sh ../resources/tmp.h
cd ..
cd test
make all

cd ../../network/output/
./uds_server &

cd ../../ctags_stuff/test/
./test
killall uds_server

