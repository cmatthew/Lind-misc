#!/usr/bin/python
# check one file is in the other
import sys


f1 = open(sys.argv[1], 'r')
f2 = open(sys.argv[2], 'r')

subset = f1.readlines()
superset = f2.readlines()

rc = 0

for l in subset:
    if l not in superset:
        rc = 1
        print "Missing:", l
    else:
        superset.remove(l)

sys.exit(rc)
