#! /bin/bash

# Test lind Grep.  Make a file system, grep a file, and make sure the same file pops back out

start_times=()
end_times=()

set -o errexit
# set -o xtrace
export REPY_PATH=~/tmp/lind/
export PATH=$PATH:~/tmp/lind/sdk/linux_x86/bin/

grep_folder=/home/lind/lind/misc/grep-2.9

ITTER=10

# first make a metadata filesystem
function setup_filesystem {
	# remove the old stuff
	path=`pwd`
	rm -rf lind.metadata
	rm -rf linddata.*

	# now make the new filesystem
 	for f in *.utf8; do 
		file_copy.py --copy $f $f > /dev/null
	done

	if [ ! -e grep.nexe ]; then

		cd $grep_folder
		make distclean
		./nacl-configure > /dev/null
		make clean all > /dev/null
		cd $path
		cp $grep_folder/src/grep ./grep.nexe
		

	fi

	if [ ! -e grep ]; then

		cd $grep_folder
		make distclean
		./configure
		make all
		cd $path

		cp $grep_folder/src/grep ./grep
	
	fi

	}

	
function run_grep {
	# run the nacl and non-nacl command and compare the output
	# $1 is the args, $2 is the file name(s)
	# to compare them, we have to clean the lind output
	# first, remove all extra lind print statements
	# then since lind returns dirs in different order when doing -r
	# run sort on the output so that the order is not an issue.

	# get rid of any old output
	rm -rfv *.out.txt
	rm -rfv output.*
	args=$1
	#run the command

	for((i = 0; i < 10; i++))
	do
		
		out=output.lind.$$
		~/tmp/lind/bin/lind $path/grep.nexe $args $2 | grep ">>" | cut -f2,3 -d \  >> $out
				
		s=$(date +%s)
		n=$(date +%N)		

		real=output.real.$$
		$path/grep $args $2 | grep ">>" | cut -f2,3 -d \  >> $real
	done
	
	rm -rf lind_results c_results
	
	mkdir lind_results c_results

	cp $out lind_results
	cp $real c_results
	
	
	return $rc
}
	

setup_filesystem
run_grep "--text Hi" "10.txt.utf8"

#run_grep "He" "10.txt.utf8"
#run_grep "He" "10609.txt.utf8 10.txt.utf8 11.txt.utf8 1342.txt.utf8 1400.txt.utf8 1661.txt.utf8 2591.txt.utf8 30601.txt.utf8 38840.txt.utf8 4300.txt.utf8 5200.txt.utf8 76.txt.utf8 98.txt.utf8" 

# We need this include because local grep starts to read
# the output files and gets in a nasty loop
#run_grep "-r --include=*.utf8 Lyon" "."
