#!/usr/bin/python2.6

import sys
import os
sys.path.append("/home/lind/tmp/lind/repy/")
import wrapped_lind_fs_calls as lind_fs_calls
from lind_fs_constants import *
import argparse


#given a filepath returns true if the file exists in the fs, else false
def check_if_file_exists(filepath):

	#try to open the file specified, if it fails, then it doens't exist
	try:
		lindfd = lind_fs_calls.open_syscall(filepath, O_RDONLY, 0)
		lind_fs_calls.close_syscall(lindfd)
		#file found, return true
		return True
	except lind_fs_calls.SyscallError:
		#file not found, return false
		return False


#goes through the inode table and checks if the files are there
def go_through_inode_table():

	files_not_found = []
	#loop through all the inodes
	for filepath in lind_fs_calls.fastinodelookuptable:
		#if the file isn't found, we want to add it to the list of 'files not found'
		if not check_if_file_exists(filepath):
			files_not_found.append(filepath)

	#if there was atleast one inconsistency
	if not files_not_found == []:
		print "There were inconsistencies.\n\nFiles that could not be found:"
		#loop through all the files that couldn't be found and print out their names
		for filepath in files_not_found:
			print filepath


#main
def main():

	#check if there is existing metadata, if there is none, create a blank one
	try:
		open("lind.metadata", 'r')
		#load the metadata
		lind_fs_calls.restore_metadata("lind.metadata")
		#start the loop to go through the table
		go_through_inode_table()
	except IOError, e:
		lind_fs_calls._blank_fs_init()


#start off with main
if __name__ == "__main__":
	main()
