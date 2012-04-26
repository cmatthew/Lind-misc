log=memoryUsageOutput
ITTER=1000

rm $log

#if a 1 is passed in, 
if test $1 -eq 1
then 
	echo "Getting native-grep memory Usage"
	for((i = 0; i < $ITTER; i++))
		do
			echo "t"`date +%s.%N` >> $log
			ps -U lind -o rss=MEM,comm=CMD,pid=PID | grep -E "grep" >> $log		
			echo "=====" >> $log
			sleep .1
			echo $i
		done

	echo "running python program now"

	./addTimes.py $log 1 > bigPrintOutLind

elif test $1 -eq 0
then
	echo "Getting lind-grep memory Usage"
	for((i = 0; i < $ITTER; i++))
		do	
			echo "t"`date +%s.%N` >> $log
			ps -U lind -o rss=MEM,comm=CMD,pid=PID | grep -E "lind|python2.6|sel_ldr" >> $log		
			echo "=====" >> $log
			sleep .1
			echo $i
		done

	echo "running python program now"

	./addTimes.py $log 0 > finalStats

else

	echo "Use ./memoryUsage.sh <1|0>, where 1 specifies native, 0 specifies lind."

fi