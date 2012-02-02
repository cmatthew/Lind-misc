#!/usr/bin/python2.6

import sys
import os
import stat
import argparse
import hashlib

# add repy install path to script
path = os.getenv("REPY_PATH")
if path == None:
    print "Error: REPY_PATH enviroment variable must be set."
    sys.exit(1)
sys.path.append(path)
path = os.path.join(path, "repy")
sys.path.append(path)

from lind_fs_constants import *

import wrapped_lind_fs_calls as lind_fs_calls

# Just a few things to note:
#
# First off, when I parse the arguments to function calls, I use a try/except statement with
# the except part only containing 'True'
# The reason I do this is because the argumentparser code EXITS when the argument is -h, and we dont
# want the program to exit. So I catch it and do nothing.


#copy a file into the lind fs
def copy_file(source, path, new_name):

    #try to open the native file, if unsuccessful print an error and return
    try:
        filedata = open(source, 'r')
    except IOError, e:
        print "Could not open the file. Error: %s" % e
        return -1

    #try to open/create the native file. if unsucessful, error and return
    try:
        mode = stat.S_IMODE(os.stat(source).st_mode)
        #print "in copy " + source+ "..." + str(mode) + "..." + str(stat.S_IRWXU)
        lindfd = lind_fs_calls.open_syscall(path + "/" + new_name, O_CREAT | O_RDWR, mode)

    except lind_fs_calls.SyscallError, e:
        print "Could not open the local file. Error: %s" % e
        return -1

    #try to write the file content to the newly file
    try:
        lind_fs_calls.write_syscall(lindfd, filedata.read())
    except lind_fs_calls.SyscallError, e:
        print "Could not write file. Error: %s" % e
        return -1


    lind_fs_calls.close_syscall(lindfd)

#pass in a path, check if it exists in the virtual fs. if it doesnt, it creates the path
def check_path_exists(path):
    path_list = path.split("/")[0:-1]#get each directory in the path, excluding the new file's name
    lind_fs_calls.chdir_syscall("/")#start at root
    path_str = "/"#this string will hold the whole path of the destination file (excluding the filename itself)

    #loop through each dir in the path
    for p in path_list:
        if(p == ""):
            continue
        path_str += (p + "/")#concat the dir to the full path string
        #try to chdir (if it fails, means the directory doesnt exist)
        try:
            lind_fs_calls.chdir_syscall(p)
        #if (when) the exception is caught, create a new dir and chdir to it
        except lind_fs_calls.SyscallError as e:

            mode = stat.S_IMODE(os.stat(path_str + p).st_mode)

            #print "in check: " + path + p + "..." + str(mode) + "..." + str(S_IRWXU)

            lind_fs_calls.mkdir_syscall(p, mode)
            lind_fs_calls.chdir_syscall(p)
    return path_str





#recursive function which adds directories as well as files
def cp_recursive(source, dest):

    new_file_name = dest.split("/")[-1]#get just the new file's name
    #check if the source file is a directory
    if(not os.path.isdir(source)):
        #not a directory, just make sure the path exists, then copy the file into the virtual FS
        path_str = check_path_exists(dest)
        copy_file(source, path_str, new_file_name)
    else:
        #if it IS a dir we need to do a little more work.
        path_str = check_path_exists(dest)#add the path up to the directory
        lind_fs_calls.chdir_syscall(path_str)#chdir to the end of that new path

        mode = stat.S_IMODE(os.stat(source).st_mode)

        #print "in cp " +  source + "/" + new_file_name + "..." + str(mode) + "..." + str(S_IRWXU)
        try:
            lind_fs_calls.chdir_syscall(new_file_name)#chdir to that new directory
        except lind_fs_calls.SyscallError:
            lind_fs_calls.mkdir_syscall(new_file_name, mode)#make a directory for the new directory
            lind_fs_calls.chdir_syscall(new_file_name)#chdir to that new directory

        #then we loop through each of the subfiles of the directory, and recursively call the cp function
        #keeping track of both the native file location as well as the virtual file location!
        for c in os.listdir(source):
            cp_recursive(source + "/" + c, dest + "/" + c)



#parses the args, calls the actual cp function
def cp_cmd(input_list):
    cur_dir = lind_fs_calls.fs_calls_context['currentworkingdirectory']#save the current directory

    #create a new argument parser, with 2 argument options (source and destination)
    parser = argparse.ArgumentParser(description='Copy source to dest')
    parser.add_argument('source', metavar='SOURCE', type=str, help='The file that you would like to copy')
    parser.add_argument('dest', metavar='DEST', type=str, help='The file you would like to copy into')

    #start off with an empty list
    cp_cmd.args = []

    #try to parse the args
    try:
        cp_cmd.args = parser.parse_args(input_list)
    except SystemExit, e:
        pass
    else:
        #the args were parsed correctly, call the actual cp function
        cp_recursive(cp_cmd.args.source, cp_cmd.args.dest);

    #it is very likely that the current working directory will be changed, so go back to where the user was before the cp
    try:
        lind_fs_calls.chdir_syscall(cur_dir)
    except lind_fs_calls.SyscallError, e:
        print "In cp_cmd. Could not cd. Error: %s" % e



#this function is just for debugging. Copies a file (NON directory) back to disk so we can diff
def cpout_cmd(source, dest):

    try:
        lindfd = lind_fs_calls.open_syscall(source, O_RDWR, 0)
    except lind_fs_calls.SyscallError, e:
        print "Couldnt open local file. Error: %s" %e
        return -1

    try:
        newfd = open(dest, 'w')
    except IOError, e:
        print "Couldnt open native file. Error: %s" % e
        return -1

    try:
        newfd.write(lind_fs_calls.read_syscall(lindfd, 1000000))
    except IOError, e:
        print "Failed to write to disk. Error: %s" % e
        return -1

    lind_fs_calls.close_syscall(lindfd)

#create a new directory
def mkdir_cmd(input_list):

    #create a new argument parser, with 1 argument options (one or more new-directory-names)
    parser = argparse.ArgumentParser(description='Creates a one or more directories with the specified names')
    parser.add_argument('directory', metavar='dir', nargs='+', type=str, help='The directory names you want to create')

    #start off with an empty list
    mkdir_cmd.args = []

    #try to parse the args
    try:
        mkdir_cmd.args = parser.parse_args(input_list)
    except SystemExit, e:
        pass
    else:
        #if they were correctly parsed, try to create a new directory for each name the user specified
        try:
            for new_dir_name in mkdir_cmd.args.directory:
                retval = lind_fs_calls.mkdir_syscall(new_dir_name, S_IRWXU)
        except lind_fs_calls.SyscallError, e:
            print "Could not mkdir. Error: %s" % e



#changes directory
def cd_cmd(input_list):
    #create a new argument parser, with 1 argument options (the directory to cd to). This arg may be null
    parser = argparse.ArgumentParser(description='Changes the current working directory to the one specified by dir')
    parser.add_argument('directory', metavar='dir', nargs='?', type=str, help='The path (relative or absolute) to cd to')

    #start with an empty list
    cd_cmd.args = []

    #try to parse the args
    try:
        cd_cmd.args = parser.parse_args(input_list)
    except SystemExit, e:
        pass
    else:
        #if the args were correctly parsed, call the chdir_syscall.
        try:
            #if there were no args passed in, cd to root, else cd to where the user wants to go
            if cd_cmd.args.directory == None:
                lind_fs_calls.chdir_syscall("/")
            else:
                print cd_cmd.args.directory
                lind_fs_calls.chdir_syscall(cd_cmd.args.directory[0])
        except lind_fs_calls.SyscallError, e:
            print "Could not cd. Error: %s" % e



#prints the children of the current directory
def md5_cmd(input_list):
    """print the md5 digest of all the files"""
    for filename in input_list:
        m = hashlib.md5()
        lindfd = lind_fs_calls.open_syscall( filename, O_CREAT | O_RDWR, 0)
        while True:
            s = lind_fs_calls.read_syscall(lindfd,4096)
            m.update(s)
            if len(s) == 0:
                break
        print m.hexdigest() + "  " + filename
            


#prints the children of the current directory
def ls_cmd(input_list):

    #create a new argument parser, with 1 argument options (the directory to ls). This arg may be null
    parser = argparse.ArgumentParser(description='Print the contents of the directory specified by dir')
    parser.add_argument('directory', metavar='dir', nargs='?', type=str, help='The path (relative or absolute) to ls')

    #start with an empty list
    ls_cmd.args = []

    #try to parse the args
    try:
        ls_cmd.args = parser.parse_args(input_list)
    except SystemExit, e:
        pass
    else:
        try:
            #if no arg was passed in, just ls the current dir, else the dir specified. We need to open a file first so we can getdents
            if ls_cmd.args.directory == None:
                lindfd = lind_fs_calls.open_syscall(lind_fs_calls.fs_calls_context['currentworkingdirectory'],O_RDONLY, S_IRWXU)#first open the file
            else:
                lindfd = lind_fs_calls.open_syscall(ls_cmd.args.directory[0],O_RDONLY, S_IRWXU)#first open the file
        except lind_fs_calls.SyscallError, e:
            print "Could not open the local file. Error: %s" % e
            return -1


        at_end = False#this will keep track of when we have visited all the nodes
        getdents_size = 20#the number of nodes to check per getdents
        ls_list = []#the list that will hold all the subfiles
        prev_ls_list_count = -1#keeps track of the list count during the previous iteration

        #while there are still files to look at, add them to the list. Once the list size is the same for 2 iterations in a row, we know there is nothing left to look at!
        while not len(ls_list) == prev_ls_list_count:
            try:
                ls_list.extend(lind_fs_calls.getdents_syscall(lindfd, getdents_size))#add the current group of <getdents_size> files to the list of all files
                prev_ls_list_count = len(ls_list)#update the count
            except lind_fs_calls.SyscallError, e:
                print "getdents failed. Error: %s" % e

        #loop through all the items added to the list
        for i in ls_list:
            name = i[1]#get the name

            if(not name == "." and not name == ".."):#we dont want to display . or ..
                sys.stdout.write(name+" ")#output the name inline
        if(not ls_list == []):
            print ""#add a new line to the end

        lind_fs_calls.close_syscall(lindfd)



#get the command and the args, then call the appropriate function
def parse_input(input_string):

    #split the input on ALL whitespace
    input_list = input_string.split()
    #this is the actual command the user wants to execute
    cmd = input_list[0]

    #call the appropriate function. All arg parsing is done in the functions
    if (cmd == "cd"):
        cd_cmd(input_list[1:])
    elif(cmd == "mkdir"):
        mkdir_cmd(input_list[1:])
    #elif(cmd == "rmdir"):
    elif(cmd == "cp"):
        cp_cmd(input_list[1:])
    elif(cmd == "cpout"):
        cpout_cmd(input_list[1:])
    elif(cmd == "ls"):
        ls_cmd(input_list[1:])
    elif(cmd == "md5sum"):
        md5_cmd(input_list[1:])
        
    else:
        print "%s is not a recognized command" % cmd


#starts the loop that looks for user input
def main():

    #check if metadata exists. if it doesnt, call _blank_fs_init, if it DOES exist call persist_metadata
    try:
        f = open("lind.metadata", 'r')

        lind_fs_calls.restore_metadata("lind.metadata")
        f.close()
    except IOError, e:
        lind_fs_calls._blank_fs_init()
    # except KeyError,e:
    #     print "Error: I have failed to open the file system. The metadata is corrupt"
    #     #TODO something better?
    #     sys.exit(1)
    #set up the arg parser to parse the args to the program
    parser = argparse.ArgumentParser(description='Allows the user to manage their lind file systems, and copy files in')
    parser.add_argument('interactive', metavar='-', nargs='?', type=str, help='If specified, the program runs in interactive mode')
    parser.add_argument('copy', metavar='cp', nargs='?', type=str, help='Runs just a single copy')

    args = parser.parse_args()

    #the main argument is the '-'. If it is specified, the user would like to run in interactive mode
    if not args.interactive:

        pass

    else:

        #request user input (the command they want to type)
        input_value = raw_input(lind_fs_calls.fs_calls_context['currentworkingdirectory'] +": ")

        #loop until the user hits exit, prompting them to enter a command at each iteration
        while((not input_value == "exit")):
            parse_input(input_value)
            input_value = raw_input(lind_fs_calls.fs_calls_context['currentworkingdirectory'] +": ")


    lind_fs_calls.persist_metadata("lind.metadata")



if __name__ == "__main__":
    main()
