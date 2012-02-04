#!/bin/bash
set -o errexit
set -o xtrace

PREFIX=andi
if [ $# != 1 ]
   then
   echo "no input file"
   exit
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
./ctags_parser.py ../output/tagstmp.h ../resources/tmp.h
mv ../output/tagstmp.h.andi "../test/tmptest.c"
cp ../output/tmp.h "../test/tmptest.h"
#./parser.py complexxxx$1
#./parser.py xxx$1


