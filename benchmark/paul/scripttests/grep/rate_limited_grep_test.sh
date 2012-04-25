#!/bin/bash

#set -o xtrace #print script lines before executed
#set -o errexit #stops whenever there is an error

num=unassigned
rm -rf lind_results c_results
for i in 10000000 1000000 10000 1000 100 10
do

num=$i

template="resource cpu 1.0\n
resource memory 25000000   # 15 Million bytes\n
resource diskused 1000000000 # 1000 MB\n
resource events 10\n
resource filewrite 10000000\n
resource fileread $num\n
resource filesopened 5000\n
resource insockets 5\n
resource outsockets 5\n
resource netsend 10000\n
resource netrecv 10000\n
resource loopsend 1000000\n
resource looprecv 1000000\n
resource lograte 100000000\n
resource random 100000000\n
resource messport 10001\n
resource connport 10001\n

call gethostbyname_ex allow\n
call sendmess allow\n
call stopcomm allow 			# it doesn't make sense to restrict\n
call recvmess allow\n
call openconn allow\n
call waitforconn allow\n
call socket.close allow 		# let's not restrict\n
call socket.send allow 			# let's not restrict\n
call socket.recv allow 			# let's not restrict\n
# open and file.__init__ both have built in restrictions...\n
call open arg 0 is junk_test.out allow 	# can write to junk_test.out\n
call open arg 1 is rb allow 		# allow an explicit read\n
call open noargs is 1 allow 		# allow an implicit read \n
call file.__init__ arg 0 is junk_test.out allow # can write to junk_test.out\n
call file.__init__ arg 1 is rb allow 	# allow an explicit read\n
call file.__init__ noargs is 1 allow 	# allow an implicit read\n
call file.close allow 			# shouldn't restrict\n
call file.flush allow 			# they are free to use\n
call file.next allow 			# free to use as well...\n
call file.read allow 			# allow read\n
call file.readline allow 		# shouldn't restrict\n
call file.readlines allow 		# shouldn't restrict\n
call file.seek allow 			# seek doesn't restrict\n
call file.write allow 			# shouldn't restrict (open restricts)\n
call file.writelines allow 		# shouldn't restrict (open restricts)\n
call sleep allow			# harmless\n
call settimer allow			# we can't really do anything smart\n
call canceltimer allow			# should be okay\n
call exitall allow			# should be harmless\n

call log.write allow
call log.writelines allow
call getmyip allow			# They can get the external IP address\n
call listdir allow			# They can list the files they created\n
call removefile allow			# They can remove the files they create\n
call randomfloat allow			# can get random numbers\n
call getruntime allow			# can get the elapsed time\n
call getlock allow			# can get a mutex"

echo -e $template > ~/tmp/lind/repy/restrictions.lind
./gre_p_test.sh $i

done








