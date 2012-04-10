#!/usr/bin/python2.6

import sys
import os
sys.path.append("/home/lind/tmp/lind/repy/")
import wrapped_lind_fs_calls as lind_fs_calls
from lind_fs_constants import *
import argparse
import stat

#there are many errors that can be found in the filesystem
#this class is sort of like an enumerator for those errors
class error_types:
    no_error = 0
    inode_not_found = 1
    linddata_not_found = 2
    mode_zero = 3
    size_incorrect = 4

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


#goes through the fastinodelookuptable and looks at the absolute path for each inode
#in the data structure. it then checks if that inode is in the file system or not.
#it prints out each inode (path) that is not found.
def check_paths_correctness():
    files_not_found = []
    #loop through all the inodes
    for filepath in lind_fs_calls.fastinodelookuptable:
        #if the file isn't found, we want to add it to the list of 'files not found'
        #I am not sure what to do with this list yet. print it out and delete the files?
        if not check_if_file_exists(filepath):
            files_not_found.append(filepath)

    #if there was atleast one inconsistency
    if not files_not_found == []:
        print "There were inconsistencies.\n\nFiles that could not be found:"
        #loop through all the files that couldn't be found and print out their names
        for filepath in files_not_found:
            print filepath


#this function handles what needs to be done if an inode cannot be found
#but is said to be the child of another inode (appears in parent_num's list of children)
def handle_inode_not_found(child_num_child_name, parent_num, full_path):
    print "File %s seems to be missing" % full_path
    del lind_fs_calls.filesystemmetadata['inodetable'][parent_num]['filename_to_inode_dict'][child_name]

    #check to see if there is a linddata.# file corresponding to the inode to delete
    try:
        open(lind_fs_calls.FILEDATAPREFIX+str(child_num), 'r').close()
    #there WAS NOT a linddata.# file. so just pass
    except IOError:
        pass
    #there WAS a linddata.# file. Add it to the 'removed_file' buffer, and delete it
    else:
        print "There is a %s.# for an inode that has been removed. It is %s%d." %(lind_fs_calls.FILEDATAPREFIX, lind_fs_calls.FILEDATAPREFIX, child_num)

#this function handles a linddata.# file not being found for a regular file
def handle_linddata_not_found_error(child_num, child_name, parent_num, full_path, make_changes_flag):
    print "%s%d cannot be found. It is required for %s" %(lind_fs_calls.FILEDATAPREFIX,child_num, full_path)
    if(make_changes_flag):
        #remove the inode from its parent's children list, and from the inode lookup table
        del lind_fs_calls.filesystemmetadata['inodetable'][parent_num]['filename_to_inode_dict'][child_name]
        del lind_fs_calls.fastinodelookuptable[full_path]

#if the mode is zero
def handle_mode_zero(child_num, child_name, full_path):
    print "%s has a mode of zero." % full_path

#if the size of the file in the lind fs does not match the size of its corresponding linddata.#
def handle_size_incorrect(child_num, child_name, full_path):
    print "file %s has a different size than its cooresponding linddata.# file (%s%d)" % (full_path, lind_fs_calls.FILEDATAPREFIX, child_num)

#recursively loop through the directory structure. From here we can do what ever checks on inodes we want
#currently we check:
#	to make sure that all children listed in a directory actually exist
#	to make sure that all regular files have a corresponding linddata.# file
def loop_through_dir_tree_recursive(inode_num, full_path, make_changes_flag):

    #check to see if the inode actually exists
    try:
        lind_fs_calls.filesystemmetadata['inodetable'][inode_num]
    except KeyError, e:
        #the inode doesn't exist, so return that to the caller
        return error_types.inode_not_found

    #check to see if the inode's mode is zero...if it IS zero, something funky has happened....
    if lind_fs_calls.filesystemmetadata['inodetable'][inode_num]['mode'] == 0:
        return error_types.mode_zero


    #if the current inode in the list is not a directory, try to open the corresponding
    #linddata.# file. If it cannot be opened (it doesnt exist), return that error
    if not stat.S_ISDIR(lind_fs_calls.filesystemmetadata['inodetable'][inode_num]['mode']):
        try:
            fd = open(lind_fs_calls.FILEDATAPREFIX+str(inode_num),'r')
            #get the file size in native FS
            fd.seek(0,2)
            size = fd.tell()

            #check if the size of the file in the lind system is the same size of the
            #corresponding linddata.# in the native OS (they should be the same)
            if(not size == lind_fs_calls.filesystemmetadata['inodetable'][inode_num]['size']):
                #if the user would like to automatically make changes, then set the size of the
                #lind file to be the size of the linddata.# file
                if(make_changes_flag):
                    lind_fs_calls.filesystemmetadata['inodetable'][inode_num]['size'] = size

                return error_types.size_incorrect

            fd.close()

        except IOError:
            #the linddata.# was not found, return that to the caller
            return error_types.linddata_not_found


    #if the current inode IS a directory, recursively call this function on each of its children
    else:
        for child_name, child_num in lind_fs_calls.filesystemmetadata['inodetable'][inode_num]['filename_to_inode_dict'].items():

            #we do not want to recursively call the function on the inode itself, its parent, or the root.
            if not child_name == ".." and not child_name == "." and not child_num == 0:

                #
                full_path = full_path + "/" + child_name
                #get the error value
                e = loop_through_dir_tree_recursive(child_num, full_path, make_changes_flag)

                #check to see if any errors were returned from the recursive calls, and call the approrpriate handler function
                if(e == error_types.inode_not_found):
                    handle_inode_not_found(child_num, child_name, inode_num, full_path)

                elif(e == error_types.linddata_not_found):
                    handle_linddata_not_found_error(child_num, child_name, parent_num, full_path, make_changes_flags)

                elif(e == error_types.mode_zero):
                    handle_mode_zero(child_num, child_name, full_path)

                elif(e == error_types.size_incorrect):
                    handle_size_incorrect(child_num, child_name, full_path)
    #if there were no other errors, return no_error
    return error_types.no_error

#main
def main():

    #create the arg parser, and add the args to parse (just -f for now)
    parser = argparse.ArgumentParser(description='Checks to see if there are any inconsistencies in the lind filesystem')
    #optional argument -f
    parser.add_argument('-f', action='store_true', help='If specified, the program automatically makes changes when it sees errors')

    args = parser.parse_args()

    #this flag will be passed through all the recursive calls, so that the program knows to
    #make changes as it sees errors. true if the -f flag was specified
    make_changes_flag = False
    if(args.f):
        make_changes_flag = True

    #check if there is existing metadata, if there is none, create a blank one

    lind_fs_calls.load_fs()
    loop_through_dir_tree_recursive(0, "", make_changes_flag)
    #check_paths_correctness()


    lind_fs_calls.persist_metadata("lind.metadata")

#start off with main
if __name__ == "__main__":
    main()
