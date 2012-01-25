import sys
import os
sys.path.append("/home/lind/tmp/lind/repy/")
import wrapped_lind_fs_calls as lind_fs_calls
from lind_fs_constants import *
import stat


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
def cp(source, dest):
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
			cp(source + "/" + c, dest + "/" + c) 
		
		
#this function is just for debugging. Copies a file (NON directory) back to disk so we can diff		
def cpout(source, dest):
	
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
	
			

#display the text of the file	
def cat(filename):
	try:#try to open the file to be cat'd
		lindfd = lind_fs_calls.open_syscall(filename, O_RDWR, 1)
	except lind_fs_calls.SyscallError as e:
		"Could not cat the file, error: \n" + e#print an error if it couldnt be
		return -1

	#TODO: lseek to get number of chars
	sys.stdout.write(lind_fs_calls.read_syscall(lindfd, 100))#print the contents of the file
		
	lind_fs_calls.close_syscall(lindfd)



#create a new directory
def mkdir(dirname):

	try:
		retval = lind_fs_calls.mkdir_syscall(dirname, S_IRWXU)
	except lind_fs_calls.SyscallError, e:
		print "Could not mkdir. Error: %s" % e
	
	
	
#print stats of the file requested
def stat_cmd(path):
	try:
		print lind_fs_calls.stat_syscall(path)
	except lind_fs_calls.SyscallError, e:
		print "Could not stat. Error: %s" % e
	
	
	
#changes directory	
def cd(path):
	try:
		lind_fs_calls.chdir_syscall(path)
	except lind_fs_calls.SyscallError, e:
		print "Could not cd. Error: %s" % e
	
	
#prints the children of the current directory	
def ls(path):
	at_end = False#this will keep track of when we have visited all the nodes
	getdents_size = 20#the number of nodes to check per getdents
	ls_list = []#the list that will hold all the subfiles
	prev_ls_list_count = -1#keeps track of the list count during the previous iteration
	lindfd = lind_fs_calls.open_syscall(path,O_RDONLY, S_IRWXU)#first open the file
	
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
def get_command(cmd):
	cur_dir = lind_fs_calls.fs_calls_context['currentworkingdirectory']#save the current directory
	args = cmd.split(" ")[1:]#get all the arguments the user wants
	cmd = cmd.split(" ")[0]#the first entry is the command the user wants to run
	
	if (cmd == "cd"):
		if(len(args) == 1):
			cd(args[0])
	elif(cmd == "mkdir"):
		if(len(args) == 1):
			mkdir(args[0])
	#elif(cmd == "rmdir"):
	#	if(len(args) ==1):
	#		rmdir(args[0])
	elif(cmd == "cp"):
		if(len(args) == 2):
			#if the first character in the destination is NOT a '/', then we know we are not dealing with an absolute path,
			#so concatenate the absolute path of the current directory with the relative path the user wants
			if( not (args[1])[0] == "/"):
				args[1] = cur_dir + "/" + args[1]
			cp(args[0], args[1])
			cd(cur_dir)#we mostlikely cd in cp, so make sure we cd back to where the user was at!
	#cpout is primarily for testing		
	elif(cmd == "cpout"):
		if(len(args) == 2):
			cpout(args[0], args[1])
			cd(cur_dir)	
	#cat is primarily for testing	
	elif(cmd == "cat"):
		if(len(args) == 1):
			cat(args[0]);
	#ls accepts a path, but defaults to the current directory
	elif(cmd == "ls"):
		if(len(args) == 1):
			ls(args[0])
		else:
			ls(".")		
	#stat accepts a path as a parameter
	elif(cmd == "stat"):
		if(len(args) ==1):
			stat_cmd(args[0])
	#if the command wasnt recognized, let the user know
	else:
		print "Invalid command"



#starts the loop that looks for user input
def main():

	#check if metadata exists. if it doesnt, call _blank_fs_init, if it DOES exist call persist_metadata
	try:
		open("lind.metadata", 'r')
		lind_fs_calls.restore_metadata("lind.metadata")	
	except IOError, e:
		lind_fs_calls._blank_fs_init()
		
	#request user input (the command they want to type)
	input_value = raw_input(lind_fs_calls.fs_calls_context['currentworkingdirectory'] +": ")
	
	#loop until the user hits exit, prompting them to enter a command at each iteration
	while((not input_value == "exit")):
		get_command(input_value)
		input_value = raw_input(lind_fs_calls.fs_calls_context['currentworkingdirectory'] +": ")

		
	lind_fs_calls.persist_metadata("lind.metadata")	
	
	
	
if __name__ == "__main__":
	main()
