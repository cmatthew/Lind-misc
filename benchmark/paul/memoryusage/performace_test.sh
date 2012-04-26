log=memoryUsageOutput
ITTER=1000

rm $log

#if a 1 is passed in, we are getting NATIVE times
if test $1 -eq 1
then 
	echo "Getting native-grep memory Usage" #helpful message
	for((i = 0; i < $ITTER; i++))
		do
			echo "t"`date +%s.%N` >> $log #get the timestamp of NOW
			ps -U lind -o rss=MEM,comm=CMD,pid=PID | grep -E "grep" >> $log #get all the processes of the ps. Since it is native, we are just looking for a grep
			echo "=====" >> $log #deliminator
			sleep .1 #we dont want to just blitz, because then we will get multiples of the SAME process ID, with different ps'
			echo $i
		done

	echo "running python program now"

	./addTimes.py $log 1 > bigPrintOutLind

#if a 0 is passed in, we are getting LIND times
elif test $1 -eq 0
then
	echo "Getting lind-grep memory Usage"
	for((i = 0; i < $ITTER; i++))
		do	
			echo "t"`date +%s.%N` >> $log
			ps -U lind -o rss=MEM,comm=CMD,pid=PID | grep -E "lind|python2.6|sel_ldr" >> $log #get processes, since it is lind, we are looking for 3 things!
			echo "=====" >> $log
			sleep .1
			echo $i
		done

	echo "running python program now"

	./addTimes.py $log 0 > finalStats

else

	echo "Use ./memoryUsage.sh <1|0>, where 1 specifies native, 0 specifies lind."

fi
