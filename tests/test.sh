#! /bin/bash
set -o errexit

## CAT
echo "Note: Testing Cat"
cd cat
./cattest.sh
echo "Note: Cat Test Passed."
cd ..


## GREP
echo "Note: Testing Grep"
cd grep
./greptest.sh
echo "Note: Grep Test Passed."
cd ..


