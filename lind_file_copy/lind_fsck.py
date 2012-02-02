#!/usr/bin/python2.6

import sys
import os
sys.path.append("/home/lind/tmp/lind/repy/")
import wrapped_lind_fs_calls as lind_fs_calls
from lind_fs_constants import *
import argparse

def main():

	try:
		open("lind.metadata", 'r')
		lind_fs_calls.restore_metadata("lind.metadata")	
	except IOError, e:
		lind_fs_calls._blank_fs_init()

	print lind_fs_calls.fastinodelookuptable



if __name__ == "__main__":
	main()
