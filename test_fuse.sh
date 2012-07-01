#!/bin/bash
set -o xtrace
mkdir $$
python lind_fuse.py $$ -f &
sleep 1
cd $$
ls -al
mkdir -p foozzzzz
cd foozzzzz
ls
cd ..
rmdir foozzzzz
ls
cd ..
fusermount -u $$
wait
