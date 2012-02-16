#! /bin/bash

# Test lind Cat.  Make a file system, cat a file, and make sure the same file pops back out

set -o errexit
# set -o xtrace
export REPY_PATH=~/tmp/lind/
export PATH=$PATH:~/tmp/lind/sdk/linux_x86/bin/
# first make a metadata filesystem with 
function setup_filesystem {
	# remove the old stuff
	path=`pwd`
	rm -rf lind.metadata
	rm -rf linddata.*

	# now make the new filesystem
 	for f in *.utf8; do 
		file_copy.py --copy $f $f > /dev/null
	done

	cd ../../cat
	make clean all > /dev/null
	cd $path

	cp ../../cat/cat.nexe .
	
	}

function run_cat {

	# get rid of any old output
	rm -rfv *.out.txt
	rm -rfv output.*
	
	#run the command
	out=output.lind.$$
	~/tmp/lind/bin/lind $path/cat.nexe 10.txt.utf8 > $out

	# run the real commnad
	real=output.real.$$
	cat 10.txt.utf8 > $real

	# filter out extra Lind messages
	# cat $out
	awk -vRS="Calling Main." '{print $0>NR".out.txt"}' $out

	# remove two two lines and bottom 3 which are normal output
	tail -n+3 1.out.txt | head -n -5  > filtered.out.txt
	# only need to do this if trace is on
    # grep -vE "^\[info\]\[Trace\]\[1\] " 2.out.txt > filtered.out.txt


	# Do they match?
	if diff $real filtered.out.txt >/dev/null ; then
		rc=0
	else
		echo "Error: Cat test failed."
		rc=1
	fi

	exit $rc
}


setup_filesystem
run_cat

