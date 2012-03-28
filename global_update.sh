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
git pull github lind

echo "updating lind components"
cd ~/lind/misc
git pull 
cd ..

#link the build script to it's old location
rm -f ./b
ln -s ./misc/b

#link the loader sctipt to it's old location
rm -f ./lind.sh
ln -s ./misc/lind.sh
./b rpc glibc sdk install repy
