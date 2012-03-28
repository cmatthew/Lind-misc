#! /bin/bash
# Given a repy file with preprocessor directives, check they are all valid


f=$1
file_path=`dirname $f`
file_name=`basename $f`

files=`grep -e ^include $f | awk '{print $2}'|tr "\\n" " "`

rc=0

for file in $files 
do
    real_f=$file_path/$file
    if [ ! -f $real_f ]
    then
	echo "Error: include $file is missing in $file_name"
	rc=1
    fi
done 

exit $rc