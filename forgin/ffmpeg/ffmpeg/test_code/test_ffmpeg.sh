#!/bin/bash

if [ $# != 3 ]
	then 
	echo "USAGE: ./test_ffmpeg.sh <number of tests> <name_of_out_put_file> <inputFile>"
	echo "EXAMPLE: ./test_ffmpeg.sh 100 run1.txt SomeMovie.avi"
	exit
fi

echo ""
echo "This program runs ffmpeg and converts the inputFile to inputFile with .mkv extension"
echo "The timings results are stored in ../test_data/"
echo ""
echo ""
echo ""

i=0
while [ $i -le $1 ]
do
	echo "FFMPEG running test: $i / $1"
	
	./../ffmpeg -v 0 -i $3 Test.mkv | grep time >> ../test_data/$2

	if diff "../test_data/Test.mkv" "Test.mkv" > /dev/null; then
		echo "passed: useless because there are timestamps and meta info" >> ../test_data/$2
	else
		echo "failed: useless because there are timestamps and meta info" >> ../test_data/$2
	fi
	rm Test.mkv

	
#	echo "LIND WGET running test: $i / $1"
	
#	lind ~/lind/misc/wget/wget-1.13-lind/src/wget.nexe localhost:8000 | grep time >> ../test_data/wget$2
	
#	if diff "../test_data/index.html" "index.html" > /dev/null; then
#		echo "passed" >> ../test_data/wget$2
#	else
#		echo "failed" >> ../test_data/wget$2
#	fi
#	rm index.html
	i=$(( $i + 1 ))
done


