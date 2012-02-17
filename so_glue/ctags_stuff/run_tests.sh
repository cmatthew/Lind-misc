#! /bin/bash

set -o errexit

#cp output/tagstmp_mm.c output/bla.c

cd /home/lind/lind/misc/so_glue/ctags_stuff/
cd test

make clean

cd ..
cd src
rm -rf ../output/*_mm*
./runme.sh ../resources/tmp.h
cd ..
cd test

#rm ../output/tagstmp_mm.c
#cp ../output/bla.c ../output/tagstmp_mm.c

make all

cd /home/lind/lind/misc/so_glue/network/src/
make clean all

cd ../../network/output/
./uds_server &
sleep 1

cd ../../ctags_stuff/test/

./test
sleep 1
killall uds_server

