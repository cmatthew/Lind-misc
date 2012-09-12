#! /bin/bash

#  Lind tester for the simple netcat tester.

set -o errexit
set -o xtrace

netcat_path=~/lind/misc/foreign/netcat-0.7.1/src/

execer=lind

function process_cleanup {
    # kill all old lind and nweb processes
    # apperntly if the process does not exist there is no way to have a
    # happy return value.

    set +o errexit
    killall -q sel_ldr
    killall -q python2.6 
    killall -q lind 
    killall -q netcat
    # killall -q -s 3 native-netcat
    set -o errexit

    sleep 1
}

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

#Compile for native platform
./configure
make all
cp $netcat_path/netcat $cwd/native-netcat
make distclean

cd $cwd

# start the server
./native-netcat -l -p 10001 127.0.0.1 &
# netstat -tulpn
# sleep 2
# c_nweb=$!
# echo $c_nweb

set +o errexit
#$execer ./netcat -l -p 10001 &

#process_cleanup
cat test_netcat.sh | $execer ./netcat -n 127.0.0.1 10001  #sebulba.cs.uvic.ca 10001

#process_cleanup

#$execer ./netcat -l -p 10001

# remove any old filesystems
rm -rf lind.metadata linddata.*
