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

	cd ../../foreign/cat
	make clean all > /dev/null
	cd $path

	cp ../../foreign/cat/cat.nexe .
	
	}

function run_cat {

	# get rid of any old output
	rm -rfv *.out.txt
	rm -rfv output.*
	
	#run the command
	out=output.lind.$$
	lind ./cat.nexe 10.txt.utf8 > $out

	# run the real commnad
	real=output.real.$$
	cat 10.txt.utf8 > $real
    mydiff=~/lind/misc/is_in_file.py 

	if  $mydiff $real $out; then
		rc=0
	else
		echo "Error: Cat test failed."
		rc=1
	fi

	exit $rc
}


setup_filesystem
run_cat

