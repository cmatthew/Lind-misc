#!/bin/bash
set -o errexit
set -o xtrace

PREFIX=andi
if [ $# != 1 ]
   then
   echo "no input file"
   exit
fi


# since ctags can be a link to etags, check that it is not!
version_file="version.output"
ctags -V /dev/null > $version_file 
if grep -FqE "emacs|Emacs" $version_file
then
    echo "Please install full ctags, no etags"
	exit 1
fi



gcc -E $1 > "../output/tmp.h"
gcc -fpreprocessed -dD -E "../output/tmp.h" > "../output/tmp2.h"

rm ../output/tmp.h

sed '/^\#/d' "../output/tmp2.h" > "../output/tmp.h"


#cat xxx$1 | grep -v __gnuc_va > tmp
#mv tmp xxx$1

#tr "\n" " " < xxx$1 > tmp
#mv tmp xxx$1

tr "\t" " " < "../output/tmp.h" > "../output/tmp2.h"

ctags --c-types=+p -x ../output/tmp2.h >	../output/tagstmp.h
sort -k 3,3n ../output/tagstmp.h > ../output/tagstmp2.h
mv ../output/tagstmp2.h ../output/tagstmp.h
#./simplify.py xxx$1


#type -P pycallgraph &>/dev/null && pycallgraph --include-timing="*"
#  || echo "PycallGraph Not Found. Skipping.";  
pycallgraph ./ctags_parser.py ../output/tagstmp.h ../resources/tmp.h ../output/tagstmp_mm.c ../output/tagstmp_mm.h

mv ../output/tagstmp.h.andi "../test/tmptest.c"
cp ../output/tmp.h "../test/tmptest.h"
#./parser.py complexxxx$1
#./parser.py xxx$1


