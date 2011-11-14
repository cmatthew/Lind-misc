#!/bin/bash
# Chris Matthews <cmatthew@cs.uvic.ca>
# update all the components of lind
#set -o xtrace
#set -o errexit

echo "updating repy"
cd ~/repyv2nacl/nacl_repy/
svn update

echo "updating nacl-glibc"
cd ~/lind/nacl-glibc/
git pull

echo "updating lind components"
cd ~/lind/misc
git pull
cp b ../b

