#! /bin/bash

#  Lind tester for the simple nweb web server.
# Server was taken from here: 
#  http://www.ibm.com/developerworks/systems/library/es-nweb/index.html
# Script compiles for C, runs server and wgets a page from it.
# It then recompile for Lind, runs again, and wgets.
#
#


set -o errexit
# set -o xtrace


function process_cleanup {
    # kill all old lind and nweb processes
    # apperntly if the process does not exist there is no way to have a
    # happy return value.

    set +o errexit
    killall -q sel_ldr
    killall -q python2.6 
    killall -q lind 
    killall -q nweb
    set -o errexit

    sleep 1
}

# which port should we run on (only some are allowed in lind sandbox).
port=9999

process_cleanup

# build C version of the webserver
make clean all type=c

# start the server
./nweb $port `pwd` &
sleep 1
c_nweb=$!
echo $c_nweb
url="http://127.0.0.1:$port/index.html"
wget -O out_native.html $url
# clean up old files
rm -rf index.html.*


process_cleanup
# Now clean up and make Lind version of the code
make clean type=c
make clean all

# remove any old filesystems
rm -rf lind.metadata linddata.*

file_copy.py --copy index.html /index.html
port=10001
url="http://127.0.0.1:$port/index.html"

lind ./nweb.nexe $port / &
sleep 6
wget -O out_lind.html $url
process_cleanup

if diff out_native.html out_lind.html; then
  echo "NWeb test Passed"
else
  echo "NWeb test failed"
fi

make clean all type=c
