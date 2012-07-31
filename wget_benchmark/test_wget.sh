#!/bin/bash
set -o errexit
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

output_path=./test_data/
mkdir -p $output_path

output_file=${output_path}${2}.real
output_file_lind=${output_path}${2}.lind
rm -rf $output_file $output_file_lind
touch $output_file
touch $output_file_lind
i=0
payload="index2.html"
while [ $i -le $1 ]
do
	echo "WGET running test: $i / $1"
	
	~/lind/misc/wget/wget-1.13/src/wget -O ${output_path}${payload} http://127.0.0.1:8000/index2.html >> $output_file
    

	if diff "${output_path}${payload}" "${payload}" > /dev/null; then
		echo "passed" >> $output_file
	else
		echo "failed" >> $output_file
	fi
	rm -v ${output_path}${payload}*

	
	echo "LIND WGET running test: $i / $1"
	
	lind ~/lind/misc/wget/wget-1.13-lind/src/wget http://127.0.0.1:8000/index2.html  >> $output_file_lind
	
	if diff "./linddata.2" "$payload" > /dev/null; then
		echo "passed" >> $output_file_lind
	else
		echo "failed" >> $output_file_lind
	fi
	rm lind.log lind.metadata linddata.*

	i=$(( $i + 1 ))

done


