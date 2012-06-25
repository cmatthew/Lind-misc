#!/usr/bin/python2.6

###
# file_copy.py
#
# Written by Paul Demchuck,
#	Modified by Chris Matthews and Stephen Tredger
#
# A tool for manipulating the lind virtual filesystem

#TODO: add rm command!

import sys
import os
import stat
import hashlib

# read / write size in bytes
BLOCK_SIZE = 4096
ALL_ARGS = "cp mkdir ls md5 cat wipe cd"

# add repy install path to script
path = os.getenv("REPY_PATH")
if path == None:
    print "Error: REPY_PATH enviroment variable must be set, using default"
    path = "/home/lind/tmp/lind/"

path = os.path.join(path, "repy/")
sys.path.append(path)

# change dir so the execfile in test server works
cur = os.getcwd()
os.chdir(path)
import lind_test_server
# and now back to where we started.
os.chdir(cur)


# copy a file into the lind fs
def copy_file(source, path, new_name):
	
    # try to open the native file, if unsuccessful print an error and return
    try:
        filedata = open(source, 'r')
    except IOError, e:
        print "Could not open the file. Error: %s" % e
        return -1
	
    # try to open/create the native file. if unsucessful, error and return
    try:
        mode = stat.S_IMODE(os.stat(source).st_mode)
        #print "in copy " + source+ "..." + str(mode) + "..." + str(stat.S_IRWXU)
        lindfd = lind_test_server.open_syscall(path + "/" + new_name, lind_test_server.O_CREAT | lind_test_server.O_RDWR, mode)
	
    except lind_test_server.SyscallError, e:
        print "Could not open the local file. Error: %s" % e
        return -1
	
    # try to write the file content to the newly file
    try:
        lind_test_server.write_syscall(lindfd, filedata.read())
    except lind_test_server.SyscallError, e:
        print "Could not write file. Error: %s" % e
        return -1
	
    lind_test_server.close_syscall(lindfd)


# pass in a path, check if it exists in the virtual fs. if it doesnt, it creates the path
def check_path_exists(path):
	path_list = path.split("/")[0:-1]#get each directory in the path, excluding the new file's name
	lind_test_server.chdir_syscall("/")#start at root
	path_str = "/"#this string will hold the whole path of the destination file (excluding the filename itself)
		
	# loop through each dir in the path
	for p in path_list:
		if(p == ""): # ignore the empty string returned from splitting root dir '/'
			continue
		path_str += (p + "/")#c oncat the dir to the full path string
		
		# try to chdir (if it fails, means the directory doesnt exist)
		try:
			lind_test_server.chdir_syscall(p)
		# if (when) the exception is caught, create a new dir and chdir to it
		except lind_test_server.SyscallError:
			
			# this seems to cause an assertion to fail!
			#try: # get the mode if the dir exists in the fs
			#	mode = stat.S_IMODE(os.stat(path_str).st_mode)
			#except OSError: # if not set to default 777
			#	mode = lind_test_server.S_IRWXA
			mode = lind_test_server.S_IRWXA
						
			lind_test_server.mkdir_syscall(p, mode)
			lind_test_server.chdir_syscall(p)
	return path_str


# recursive function which adds directories as well as files
def cp_recursive(source, dest):
	
	new_file_name = dest.split("/")[-1]#get just the new file's name
	
    # check if the source file is a directory
	if(not os.path.isdir(source)):
	
		# keep same name if none specified
		if not new_file_name:
			new_file_name = source.split("/")[-1]
	
        # not a directory, just make sure the path exists, then copy the file into the virtual FS
		path_str = check_path_exists(dest)# we should create the path if it doesnt exist (but print a warning)!
		copy_file(source, path_str, new_file_name)
	else:
        # if it IS a dir we need to do a little more work.
		path_str = check_path_exists(dest)#add the path up to the directory
		lind_test_server.chdir_syscall(path_str)#chdir to the end of that new path
		
		mode = stat.S_IMODE(os.stat(source).st_mode)
		
        #print "in cp " +  source + "/" + new_file_name + "..." + str(mode) + "..." + str(S_IRWXU)
		try:
			lind_test_server.chdir_syscall(new_file_name)#chdir to that new directory
		except lind_test_server.SyscallError:
			lind_test_server.mkdir_syscall(new_file_name, mode)#make a directory for the new directory
			lind_test_server.chdir_syscall(new_file_name)#chdir to that new directory
		
        # then we loop through each of the subfiles of the directory, and recursively call the cp function
        #  keeping track of both the native file location as well as the virtual file location!
		for c in os.listdir(source):
			cp_recursive(source + "/" + c, dest + "/" + c)


# copy the source file(s) into virtual dest
def cp_cmd(source, dest):
	
	# save the current directory
	dir = lind_test_server.fs_calls_context['currentworkingdirectory']
	cp_recursive(source, dest)
	cd_cmd(dir) # cd back as we may have changed


#this function is just for debugging. Copies a file (NON directory) back to disk so we can diff
def cpout_cmd(source, dest):
	
    try:
        lindfd = lind_test_server.open_syscall(source, lind_test_server.O_RDWR, 0)
    except lind_test_server.SyscallError, e:
        print "Couldnt open local file. Error: %s" %e
        return -1
	
    try:
        newfd = open(dest, 'w')
    except IOError, e:
        print "Couldnt open native file. Error: %s" % e
        return -1
	
    try:
        newfd.write(lind_test_server.read_syscall(lindfd, 1000000))
    except IOError, e:
        print "Failed to write to disk. Error: %s" % e
        return -1
	
    lind_test_server.close_syscall(lindfd)


#create a new directory
def mkdir_cmd(dir_list):

	assert(len(dir_list))
	try:
		for new_dir_name in dir_list:
			# this follows paths and correctly creates subdirs and such!
			lind_test_server.mkdir_syscall(new_dir_name, lind_test_server.S_IRWXU)
	except lind_test_server.SyscallError, e:
		print "Could not mkdir. Error: %s" % e


#changes directory
def cd_cmd(dir):

	assert(dir)
	try:
		lind_test_server.chdir_syscall(dir)
	except lind_test_server.SyscallError, e:
		print "Could not cd. Error: %s" % e


# calc and print md5 to stdout
def md5_cmd(input_list):
	"""print the md5 digest of all the files"""
	for filename in input_list:
		m = hashlib.md5()
		lindfd = lind_test_server.open_syscall( filename, lind_test_server.O_CREAT | lind_test_server.O_RDWR, 0)
		try:
			while True:
				s = lind_test_server.read_syscall(lindfd, BLOCK_SIZE)
				m.update(s)
				if len(s) == 0:
					break
			print m.hexdigest() + "  " + filename
		except lind_test_server.SyscallError as e:
			print "Could not read file %(f)s. Error: %(err)s" % {'f': filename, 'err': e}


# print all files to stdout
def cat_cmd(input_list):
	"""print the contents of all the files"""
	for filename in input_list:
	
		if len(input_list) > 1:
			print filename + ":"
	
		lindfd = lind_test_server.open_syscall( filename, lind_test_server.O_CREAT | lind_test_server.O_RDWR, 0)
		try:
			while True:
				s = lind_test_server.read_syscall(lindfd, BLOCK_SIZE)
				print s
				if len(s) == 0:
					break
		except lind_test_server.SyscallError as e:
			print "Could not read file %(f)s. Error: %(err)s" % {'f': filename, 'err': e}

		
# clears all files and metadata
def wipe_cmd():
	"""wipe this file system"""
	cd_cmd("/") # cd so we dont get stuck in a non existant file post wipe!
	lind_test_server._blank_fs_init()
	print "Filesystem wiped"


#prints the children of the current directory
def ls_cmd(dir_list):

	# we want to print the dirname if we are ls'ing multiple dirs
	print_dir = 0
	if len(dir_list) > 1:
		print_dir = 1
	
	for dir in dir_list:
		try:
			lindfd = lind_test_server.open_syscall(dir, lind_test_server.O_RDONLY, lind_test_server.S_IRWXU) #first open the file
		except lind_test_server.SyscallError, e:
			print "Could not open the local file. Error: %s" % e
			return -1
		
		getdents_size = 20 #the number of nodes to check per getdents
		ls_list = [] #the list that will hold all the subfiles
		prev_ls_list_count = -1#keeps track of the list count during the previous iteration
		
		#while there are still files to look at, add them to the list. Once the list size is the same for 2 iterations in a row, we know there is nothing left to look at!
		while not len(ls_list) == prev_ls_list_count:
			try:
				prev_ls_list_count = len(ls_list)#update the count
				ls_list.extend(lind_test_server.getdents_syscall(lindfd, getdents_size))#add the current group of <getdents_size> files to the list of all files
			except lind_test_server.SyscallError, e:
				print "getdents failed. Error: %s" % e
		
		if print_dir:
			sys.stdout.write(dir+": ")#output the name inline

		#loop through all the items added to the list
		for i in ls_list:
			name = i[1]#get the name
			
			if(not name == "." and not name == ".."):#we dont want to display . or ..
				sys.stdout.write(name+" ")#output the name inline
		if(not ls_list == []):
			print ""#add a new line to the end
		
		lind_test_server.close_syscall(lindfd)


# given an input string determines what help to print
def print_help(args):

	if 'inter' in args or 'cmd' in args:
		print "file_copy help!"
	
	if 'cmd' in args:
		print "command line usage: [-cmd [arg ...] ...] | [source dest]"
		print "\tsource and dest with no flags is identical to -cp source dest"
		print "\tcan specify multiple -cmd's which will be performed in the order they appear"
		print "\toptions: -i | -interactive, -cp | -copy, -ls, -cd, -cat, -md5 | -md5sum, -w | -wipe, -h | -help"
	if 'inter' in args:
		print "interactive usage: cmd [arg ...]"
		print "\tuse -i or -interactive to enter interactive mode"
		
	if 'inter' in args or 'cmd' in args:
		print "\ncommands:"
	else:
		sys.stdout.write("usage: ")
		
	if 'cp' in args:
		print "cp source dest\tcopies a source file or directory into the virtual filesystem at dest, (works like unix cp -r)"
	if 'ls' in args:
		print "ls [dir ...]\tlists contents of specified directories, defaults to the current dir"
	if 'cd' in args:
		print "cd [dir]\tchanges the working directory to dir, defaults to root"
	if 'mkdir' in args:
		print "mkdir dir [...]\tcreates directories"
	if 'cat' in args:
		print "cat file [...]\tprints the contents of files"
	if 'md5' in args: 
		print "md5 file [...]\tcomputes md5 sum of files and prints"
	if 'wipe' in args:
		print "wipe, erases the current virtual filesystem"
		

#get the command and the args, then call the appropriate function
def parse_input(input_string):
	
    #split the input on ALL whitespace
	input_list = input_string.split()
	if len(input_list) == 0:
		return
		
	#this is the actual command the user wants to execute
	cmd = input_list[0]
	
    #cd only takes one arg, so ignore any extra
	if (cmd == "cd"):
		if len(input_list) < 2:
			cd_cmd("/")
		else:
			cd_cmd(input_list[1])
			
	#mkdir makes dirs for all args listed
	elif(cmd == "mkdir"):
		if len(input_list) < 2:
			print_help("mkdir")
			return
		mkdir_cmd(input_list[1:])
		
	elif(cmd == "rm"):
		print "Not yet implemented!"
	
	# we need 3 args for copy
	elif(cmd == "cp"):
		if len(input_list) != 3:
			print_help("cp")
			return
		cp_cmd(input_list[1], input_list[2])
		
	elif(cmd == "cpout"):
		cpout_cmd(input_list[1:])
		
	# ls directories
	elif(cmd == "ls"):
		if len(input_list) < 2:
			ls_cmd(lind_test_server.fs_calls_context['currentworkingdirectory'])
		else:
			ls_cmd(input_list[1:])
			
	# compute md5 sum for files
	elif(cmd == "md5sum" or cmd == "md5"):
		if len(input_list) < 2:
			print_help("md5")
			return
		md5_cmd(input_list[1:])
		
	# cat we output all files specified to stdout
	elif(cmd == "cat"):
		if len(input_list) < 2:
			print_help("cat")
			return
		cat_cmd(input_list[1:])
		
	# if we see wipe, ignore everything else and wipe!
	elif(cmd == "wipe"):
		wipe_cmd()
		
	elif (cmd == "help"):
		print_help(ALL)
	
	else:
		print "%s is not a recognized command. type 'help' to see commands" % cmd
	

# returns the number of list elements before we encounter one starting with '-'
def get_num_args(args):

	num = 0
	if len(args) == 0:
		return num
		
	while True:
		if num >= len(args):
			break
		arg = args[num]
		# realisically len(arg) should always be greater than 0 but whatevs
		if len(arg) == 0 or arg[0] == '-':
			return num
		else:
			num += 1

	return num
	

#starts the loop that looks for user input
def main():
	
	lind_test_server.load_fs()
	
	inter = 0
	no_args = 1
	
	# with no arguments we go into interactive mode
	if len(sys.argv) == 1:
		inter = 1
	else:
		# with 2 args we may want to do a copy
		if len(sys.argv) == 3:
			# if the args dont start with a '-' we want to do a copy
			if not ('-' == sys.argv[1][0] or '-' == sys.argv[2][0]):
				cp_cmd(sys.argv[1], sys.argv[2])
				return # we are done now
		
		# iterate over the args in a non pythonish way 
		#	so we can jump around when we need to
		i = 1
		while i<len(sys.argv):
			arg = sys.argv[i]
			
			# interactive mode specified
			if (arg == '-i' or arg == '-interactive'):
				inter = 1
		
			# we want to copy some files so get the source and dest
			elif (arg == '-cp' or arg == '-copy'):
				no_args = 0		
				if (i+2 < len(sys.argv)):
					cp_cmd(sys.argv[i+1], sys.argv[i+2])
					i += 1 # move one forward so after the loop we have moved 2 args
				else:
					print_help("cp")
					
			# we want to ls something
			elif (arg == '-ls'):
				no_args = 0
				num_args = get_num_args(sys.argv[i+1:])
				if num_args:
					ls_cmd(sys.argv[i+1:i+num_args+1]) # num_args+1 as its [inclusive:exclusive]
				else:
					ls_cmd(lind_test_server.fs_calls_context['currentworkingdirectory'])
				i += num_args
				
			# we want to read some files
			elif (arg == '-cat'):
				no_args = 0
				num_args = get_num_args(sys.argv[i+1:])
				if num_args:
					print num_args
					cat_cmd(sys.argv[i+1:i+num_args+1])
					i += num_args
				else:
					print_help("cat")

			# we want to do an md5 sum
			elif (arg == '-md5' or arg == '-md5sum'):
				no_args = 0
				num_args = get_num_args(sys.argv[i+1:])
				if num_args:
					md5_cmd(sys.argv[i+1:i+num_args+1])
					i += num_args
				else:
					print_help("md5")

			# we want to wipe the filesystem
			elif (arg == '-w' or arg == '-wipe'):
				no_args = 0
				wipe_cmd()

			# we want some help!
			elif (arg == '-h' or arg == '-help'):
				no_args = 0
				print_help(ALL_ARGS + " inter cmd")

			# next arg
			i += 1

	# we now enter interactive mode if specified
	if inter:	
		no_args = 0
		print "interactive mode, type 'exit' to exit"
		#request user input (the command they want to type)
		input_value = raw_input(lind_test_server.fs_calls_context['currentworkingdirectory'] +": ")
	
		#loop until the user hits exit, prompting them to enter a command at each iteration
		while((not input_value == "exit")):
			parse_input(input_value)
			input_value = raw_input(lind_test_server.fs_calls_context['currentworkingdirectory'] +": ")
	
	# if we didnt find any usable args print usage
	if no_args:
		print_help(ALL_ARGS + " inter cmd")

	lind_test_server.persist_metadata("lind.metadata")


if __name__ == "__main__":
    main()
