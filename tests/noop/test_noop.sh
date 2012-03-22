#! /bin/bash
set -o errexit

make clean all
lind ./noop.nexe
