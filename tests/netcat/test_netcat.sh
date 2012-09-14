#! /bin/bash

#  Lind tester for the simple netcat tester.

set -o errexit
set -o xtrace

source ../common.sh

netcat_path=~/lind/misc/foreign/netcat-0.7.1/src/

execer=lind

# which port should we run on (only some are allowed in lind sandbox).
port=10001

process_cleanup

# build C version of the webserver
cwd=`pwd`
cd $netcat_path


cd ..
# Compile for Lind
./nacl-configure
make clean all
#cleanup then compile for native machine
cp $netcat_path/netcat $cwd
make clean distclean

# while we are debugging, fix up repy
b repy
# #Compile for native platform
# ./configure
# make all
# cp $netcat_path/netcat $cwd/native-netcat
# make distclean

cd $cwd

# start the server
#./native-netcat -l -p 10001 127.0.0.1 &
# netstat -tulpn
# sleep 2
# c_nweb=$!
# echo $c_nweb

set +o errexit
#$execer ./netcat -l -p 10001 &

#process_cleanup
cat test_netcat.sh | $execer ./netcat -c sebulba.cs.uvic.ca 10001

#process_cleanup

#$execer ./netcat -l -p 10001

# remove any old filesystems
rm -rf lind.metadata linddata.*
