#!/bin/bash
set -o errexit
set -o xtrace

PREFIX=andi
if [ $# != 1 ]
   then
   echo "no input file"
   exit
fi


gcc -E $1 > $PREFIX$1
gcc -fpreprocessed -dD -E $PREFIX$1 > xxx$1
rm $PREFIX$1

sed '/^\#/d' xxx$1 > tmp
mv tmp xxx$1

#cat xxx$1 | grep -v __gnuc_va > tmp
#mv tmp xxx$1

#tr "\n" " " < xxx$1 > tmp
#mv tmp xxx$1

tr "\t" " " < xxx$1 > tmp
mv tmp xxx$1


ctags --c-types=+p -x xxx$1 >	tags$1
sort -k 3,3n tags$1 > tmp
mv tmp tags$1
#./simplify.py xxx$1
#./ctags_parser.py simplexxx$1
#./parser.py complexxxx$1
#./parser.py xxx$1


