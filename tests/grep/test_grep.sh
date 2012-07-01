#! /bin/bash

# Test lind Grep.  Make a file system, grep a file, and make sure the same file pops back out

set -o errexit
set -o xtrace
export REPY_PATH=~/tmp/lind/
export PATH=$PATH:~/tmp/lind/sdk/linux_x86/bin/


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

    mkdir -p dev
    touch ./dev/null
    file_copy.py --copy ./dev/null /

    
	if [ ! -e grep.nexe ]; then

		cd ../../foreign/grep-2.9
		make distclean
		./nacl-configure > /dev/null
		make clean all > /dev/null
		cd $path
 		cp ../../foreign/grep-2.9/src/grep ./grep.nexe
	fi

	if [ ! -e grep ]; then

		cd ../../foreign/grep-2.9
		make distclean
		./configure
		make all
		cd $path

		cp ../../foreign/grep-2.9/src/grep ./grep
	
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
	rm -rf *.out.txt
	rm -rf output.*
	args=$1

    echo "Grep test: grep $args $2"

	#run the command
	out=output.lind.$$
	lind ./grep.nexe $args $2 > $out

	# run the real commnad
	real=output.real.$$
    new_args=${2//\//.\/}
    echo ">>>>>>>>>>> $new_args"
	./grep $args $new_args > $real

    mydiff=~/lind/misc/is_in_file.py 

	if $mydiff $real $out ; then
		rc=0
	else
		cat $out
		echo "Error: Grep test failed."
		exit 1
	fi

	rm -rf *.out.txt
	rm -rf output.*

	return $rc
}


setup_filesystem
run_grep "--text Hi" "/10.txt.utf8"
run_grep "He" "/10.txt.utf8"
run_grep "He --no-filename" "/10609.txt.utf8 /10.txt.utf8 /11.txt.utf8 /1342.txt.utf8 /1400.txt.utf8 /1661.txt.utf8 /2591.txt.utf8 /30601.txt.utf8 /38840.txt.utf8 /4300.txt.utf8 /5200.txt.utf8 /76.txt.utf8 /98.txt.utf8" 

# We need this include because local grep starts to read
# the output files and gets in a nasty loop
# run_grep "-r --no-filename --include=*.utf8 Lyon" "/"
