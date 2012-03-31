#!/bin/bash

if [ $# != 2 ]
	then 
	echo "USAGE: ./test_wget.sh <number of tests> <name_of_out_put_file>"
	echo "EXAMPLE: ./test_wget.sh 100 run1.txt"
	exit
fi

echo ""
echo "This program runs wget and connects to localhost:8000"
echo "The timings results are stored in ../test_data/"
echo ""
echo ""
echo ""

i=0
while [ $i -le $1 ]
do
	echo "running test: $i / $1"
	i=$(( $i + 1 ))
	../src/wget localhost:8000 | grep time >> ../test_data/$2

	if diff "../test_data/index.html" "index.html" > /dev/null; then
		echo "passed" >> ../test_data/$2
	else
		echo "failed" >> ../test_data/$2
	fi
	rm index.html	
done


