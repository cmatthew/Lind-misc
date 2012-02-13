#!/usr/bin/python

"""
ctags_parser.py
	This file parses ctags output file and produces compilable C code.
	It produces:
	
		1) *.h	/* rework the naming of files later */ 
		2) *client.c 
		3) *server.c
	
	This followes the rpcgen example of producing one header, client and server
	files. 

	Copyright (c) 2012 - NOW	Andi Bergen

	This file comes with no guarantees or warranties, use at your own risk.
"""

import sys
import os

SYM_NAME = 0
TYPE = 1
LINE_NUM = 2
SOURCE_FILE = 3
SIGNATURE = 4
OUT_PATH = "../output/"
MM_AUTOGEN = "" # "../output/mm_autogen.c"
MM_CODE = ""

MM_C_OUT = ""
MM_H_OUT = ""
CUR_CALL_NUM = 0
CALL_NUM_DICT = dict()

def _cp_my_debug (message) :
	""" cp_my_debug --
	#	Prints message to console
	# Arguments:
	#	message	text to be printed to console
	# Result: 
	#	message is printed to the console
	"""
	print message


def _cp_my_open_file(target, mode = 'r'):
	""" cp_my_open_file
	#	Opens target file
	# Arguments:
	#	target 	file to open
	#	mode	mode of open, defaults to read only
	"""
	return open(target, mode)


def _cp_my_close_file(target):
	""" cp_my_close_file
	#	Closes target file
	# Arguments:
	#	target	file to close
	"""
	target.close()


def cp_cleanup(strings) :
	""" cp_cleanup
	#	Removes the unneeded colums from ctags, removes newline at end of line
	# Arguments:
	#	strings	each line of ctags output is parsed and split into columns
	#		strings ['SYM_NAME','TYPE','LINE_NUM','SOURCE_FILE','SIGNATURE']
	# Results:
	#	Cleaned up data structure, containing only the columns we need
	"""
#	del strings[2]
	strings[-1] = strings[-1].split(";")[0]
	if strings[-1][-1] == '\n':
		strings[-1] = strings[-1][:-1]
	
	return strings

def cp_deserialize(deserialize_me) :
	""" cp_serialize
	# Produces code that seriailzed all parameters coming in
	# Arguments:
	#	serialize_me	function signature
	# Result:
	#	ser_code	C code for serialization
	"""
	_cp_my_debug(serialize_me)
	tmp = serialize_me.split("(")[1].split(")")[0]
	tmp = tmp.split(',')
	tmp2 = []
	for item in tmp:
		if "void" in item:
			tmp2.append("call_num /*FIX ME*/")
		else:
			tmp2.append(item.split()[-1])
	tmp = tmp2
	ser_code = ""
	ser_code += "\tint nbytes;\n\tnbytes = 0;\n"
	# msg_size
	ser_code += '\tmemcpy(&buffer[nbytes], &my_s_size, sizeof(my_s_size));\n'
	ser_code += '\tnbytes += sizeof(my_s_size);\n'
	# call_num
	ser_code += '\tmemcpy(&buffer[nbytes], &call_num, sizeof(call_num));\n'
	ser_code += '\tnbytes += sizeof(call_num);\n'
	# version_num ==> faked
	ser_code += '\tnbytes += 4;\n '
	# flags => faked
	ser_code += '\tnbytes += 4;\n '
	# num_of_args
	ser_code += '\tmemcpy(&buffer[nbytes], &num_of_args, sizeof(num_of_args));\n'
	ser_code += '\tnbytes += sizeof(num_of_args);\n'
	for i in range(1, len(tmp)-1) :
		if "*" in tmp[i] and not "FIX" in tmp[i] and not "__" in tmp[i]:
			ser_code += '\tmemcpy(&buffer[nbytes], &'+ tmp[i].replace('*', "") +\
				', sizeof('+str(tmp[i]) +'));\n'
			ser_code += '\tnbytes += sizeof('+tmp[i].replace('*', "") +');\n'
		else :
			ser_code += '\tmemcpy(&buffer[nbytes], &'+ tmp[i]+', sizeof('+\
				str(tmp[i])+'));\n'
			ser_code += '\tnbytes += sizeof('+tmp[i]+');\n'
	
	return ser_code 


def cp_serialize(serialize_me) :
	""" cp_serialize
	# Produces code that seriailzed all parameters coming in
	# Arguments:
	#	serialize_me	function signature
	# Result:
	#	ser_code	C code for serialization
	"""
	_cp_my_debug(serialize_me)
	tmp = serialize_me.split("(")[1].split(")")[0]
	tmp = tmp.split(',')
	tmp2 = []
	for item in tmp:
		if "void" in item:
			tmp2.append("call_num /*FIXME*/")
		else:
			tmp2.append(item.split()[-1])
	tmp = tmp2
	ser_code = ""
	ser_code += "\tint nbytes;\n\tnbytes = 0;\n"
	# msg_size
	ser_code += '\tmemcpy(&buffer[nbytes], &my_s_size, sizeof(my_s_size));\n'
	ser_code += '\tnbytes += sizeof(my_s_size);\n'
	# call_num
	ser_code += '\tmemcpy(&buffer[nbytes], &call_num, sizeof(call_num));\n'
	ser_code += '\tnbytes += sizeof(call_num);\n'
	# version_num ==> faked
	ser_code += '\tnbytes += 4;\n '
	# flags => faked
	ser_code += '\tnbytes += 4;\n '
	# num_of_args
	ser_code += '\tmemcpy(&buffer[nbytes], &num_of_args, sizeof(num_of_args));\n'
	ser_code += '\tnbytes += sizeof(num_of_args);\n'
	for i in range(1, len(tmp)-1) :
		if "*" in tmp[i] and not "FIX" in tmp[i] and not "__" in tmp[i]:
			ser_code += '\tmemcpy(&buffer[nbytes], &'+ tmp[i].replace('*', "") +\
				', sizeof('+str(tmp[i]) +'));\n'
			ser_code += '\tnbytes += sizeof('+tmp[i].replace('*', "") +');\n'
		else :
			ser_code += '\tmemcpy(&buffer[nbytes], &'+ tmp[i]+', sizeof('+\
				str(tmp[i])+'));\n'
			ser_code += '\tnbytes += sizeof('+tmp[i]+');\n'
	
	return ser_code 



def cp_write_mm_magic_c(value):
	""" _cp_write_mm_magic_c
	# Writes contents of to the MM_C_OUT (serialization and mashalling)
	# Arguments:
	#	value	stub forserialization <foo ret> serialize_<foo>(<foo args>)
	# Result:
	#	sideeffects happen in the globablly specified file MM_C_OUT
	"""
	target = _cp_my_open_file(MM_C_OUT, 'a')
	target.write(value[0:-2])
	target.write("{\n\t")
	# initialize buffer
	target.write('memset(buffer, \'\\0\', BUF_SIZE);\n')
	# set up vars
	target.write('int my_s_size;\nmy_s_size = BUF_SIZE;\n')
	# populate common fields
	target.write(cp_serialize(value))
	target.write("\treturn cli_connect_buffer(buffer);\n}\n\n")
	_cp_my_close_file(target)


def cp_write_mm_magic_h(value):
	""" _cp_write_mm_magic_h
	# Writes contents of to the MM_H_OUT (header for serialization stubs)
	# Arguments:
	#	value	stub forserialization <foo ret> serialize_<foo>(<foo args>)
	# Result:
	#	sideeffects happen in the globablly specified file MM_H_OUT
	"""

	# open file for append 
	target = _cp_my_open_file(MM_H_OUT, 'a')
	# write out the target 
	target.write(value)
	_cp_my_close_file(target)

def cp_write_headers(includes, target_file) :
	""" cp_write_headers
	# Writes header includes into the specified file
	# Arguments:
	#	includes	string, intended to be include statements for a C file
	#	target_file	the file to which the string will be written
	# Result:
	#	side effects happen in the specified target_file
	"""
	target = _cp_my_open_file(target_file, 'a')
	target.write(includes+"\n")
	_cp_my_close_file(target)
	### write include header to MM_C_OUT and MM_H_OUT


def cp_fill_MM_CODE(info):
	""" cp_fill_MM_CODE
	# Generates the middle magic implementation of the serialization stubs
	# 	Does some clean up and checking of the string to make sure it compiles
	# Arguments:
	#	info	parsed list of the function signature including arguments
	# Result:
	#	side effects are at the global variable MM_CODE
	"""
	global MM_CODE
	global CUR_CALL_NUM
	
	# get info needed to write out strings
	ret_type = info[0].split()[0:-1]
	info[0] = info[0].split()[-1]
	
	mm_sig = ""
	i = 0
	if '*' in info[0]:
		mm_sig += str(ret_type[-1]) + " *serialize_"\
			+str(info[0].replace('*', "")) + "("
	else :
		mm_sig += str(ret_type[-1]) + " serialize_" + str(info[0])+ "("
	offset = 2 
	i = 1 
	while i < len(info):
		if i == 1 :
			mm_sig += "int num_of_args"
			i += 1
			continue
		if (i - offset) % 3 == 0: 
			i += 1
			mm_sig += ", int arg" + str(i/(offset+1)) + "_size, "
			continue
		if "void" in info[i+1]:
			mm_sig += "int x/*FIXME*/"
		else:
			mm_sig += info[i].replace("'", "").replace("[", "").replace(",", "")\
				.replace("]","")
			mm_sig += " " + info[i+1]#.replace('*', "")
		i += 2
	
	mm_sig += ", int call_num" 
	mm_sig += ");\n"
	# write sig out into *.h and *.c then populate *.c
	cp_write_mm_magic_c(mm_sig)
	cp_write_mm_magic_h(mm_sig)
	MM_CODE += mm_sig
	

def cp_middle_magic(sig) :
	""" cp_middle_magic
	# Generates the code/function to connect client and server.
	# Constructs a function with the arguments that need to be send
	# Arguments:
	#	sig	function signature as provided by ctags
	# Result:
	# 	ret_str	C code to call function in my library with the right
	#		number of arguments to be sent accross the network. Local impl!!!
	"""
	global CUR_CALL_NUM
	global CALL_NUM_DICT
	ret_str = ""
	# catch all, it appears to work ????
	if "int x" in sig or "" in sig: 
		# disect the signature into its components
		#	autogen_info[0]: function name
		#	autogen_info[1]: num_of_arguments
		#	autogen_info[2;-1]: [sizeof(arg), type, arg]
		#
		autogen_info = []
		# getting the function name and return type
		tmp = sig.split('(')
		autogen_info.append(tmp[0])
		# getting the function arguments
		tmp2 = tmp[1].split(')')[0].split(',')
		tmp = tmp2
		# appending with the num_of_args 
		autogen_info.append(str(len(tmp)))
		
		#populating the structure with information from the signature
		#	num_of_args, {sizeof, arg}
		for item in tmp:
			autogen_info.append("sizeof("+str(item.split()[-1])+")")
			autogen_info.append(str(item.split()[0:-1]))
			autogen_info.append(str(item.split()[-1]))
		# write stuff out into the main implementation output file
		tmp = ""
		if '*' in autogen_info[0]:
			tmp += "*serialize_" + autogen_info[0].split()[-1].replace('*', "")+"("
		else : 
			tmp += "serialize_" + autogen_info[0].split()[-1]+"("
		for i in range(0, len(autogen_info)):
				
			if i % 3 == 0:
				continue
			if i != 1:
				tmp += ", "
			# User inspection required to figure out what is happening
			if "void" in autogen_info[i]:
				tmp += "0/*FIX ME*/"
				# we can generally infer what happens (hopefully)
				# pointers don't need the * in this case
			elif "*" in autogen_info[i] :
				tmp += autogen_info[i].replace('*', "")
			else :
				tmp += (autogen_info[i])
		tmp += ", " + str(CUR_CALL_NUM)
		CALL_NUM_DICT[CUR_CALL_NUM] = str(autogen_info[0])
		CUR_CALL_NUM += 1
		tmp += ");\n"

		#write to contain serialize_foo() stub. middle_magic_<*.h> will
		# be generated automatically
		cp_fill_MM_CODE(autogen_info)
		
		# return value will be appended to the local implementation of the 
		# implementation.
		ret_str += "return " + tmp 	
	return ret_str


def cp_function_middle(signature) :
	""" cp_function_middle
	# Deals with the middle of the function. A function should look like this:
	#	<signature> {
	#	[<connection magic>]
	#	return <client-server-call()>;
	#	}
	#	This should also take care of the connection magic and make sure the 
	#	serialization is happening correctly.
	# Arguments:
	#	signature	signature of the function as delivered by ctags
	# Result:
	#	body of the function, including the return statement (i.e the part
	#	between the '{' '}'
	"""
	# kick this function out it doesn't need to be here since it does nothing,
	# just call _cp_middle_magic(signature) directly
	middle = ""
	middle += cp_middle_magic(signature)
	return middle



def cp_get_typedef(signature) :
	""" _cp_get_typedef
	# Inputs single line typedefs that are not structs into the C file
	# Arguments:
	#	signature	signature as delivered by ctags
	# Result:
	#	<signature>;"
	"""
	return signature +";\n"
	

def cp_get_fstring(signature) :
	""" cp_get_fstring
	# Complete function is returned
	# Arguments:
	#	signature	string from SIGNATURE colum of ctags output
	# Result:
	#	C code of the enture function, signature and body
	"""
	function = str(signature)+" {\n"
	function += cp_function_middle(signature)
	return function+"\n}\n"


def cp_user_includes(original_c_file) :
	""" cp_user_includes
	# Parses user code to include the same header files. Only searches for
	#	includes of the format #include <foo> (not #include "foo"). Also
	#	include middle_magic.h => NOT ANYMORE
	# Arguments:
	#	original_c_file	C code from user that uses standard library
	# Result:
	#	string of includes: #include <foo.h>
	"""
	ret_s = ""
	#ret_s = '#include "../../network/src/middle_magic.h"\n'
	user_code = _cp_my_open_file(original_c_file)
	### go through it line by line and check for #include
	for line in user_code:
		if "#include" in line:
			ret_s += line 


	_cp_my_close_file(user_code)
	return ret_s

def cp_write_c(lol, filename, orig_c_file) :
	""" cp_write_c
	# Write c code, typedefs first, then structs, then functions
	# Arguments:
	#	lol	list of lists, a line by line representation of ctags output
	#	filename	filename of header file, used to construct name of output file
	#	orig_c_file	original user C code
	# Result:
	#	New file in filesystem, client code of RPC with fully compilable C code
	"""
	c_code = ""
	### include the same headers that the user prog included
	c_code += cp_user_includes(orig_c_file)
	cp_write_headers('#include "' + MM_H_OUT + '"\n', MM_C_OUT)
	cp_write_headers(c_code, MM_H_OUT)
	cp_write_headers("#include <string.h>\n#include <stdio.h>\n\n", MM_C_OUT)
	cp_write_headers("#define BUF_SIZE 4094\nchar buffer[BUF_SIZE];\n", MM_C_OUT)
	c_code +=  '#include "'+ MM_H_OUT +'"\n'
	### deals with typedefs that are not structs
	#member = False """ do I need to look into this """
	for i in range(len(lol)) :
		if lol[i][TYPE] == "typedef" and lol[i-1][TYPE] != "member":
			c_code += cp_get_typedef(lol[i][SIGNATURE])
	### deals with structs => Probably wont need this anymore
	#"""for i in range(len(lol)):
	#	if lol[i][TYPE] == "struct":
	#		c_code += cp_get_struct(lol, i)"""
	### deals with prototypes
	for item in lol :
		if item[TYPE] == "prototype":
			c_code += cp_get_fstring(item[SIGNATURE])
	os.system("touch "+OUT_PATH+filename+".andi")
	f_out = _cp_my_open_file(OUT_PATH+filename+".andi", "w+")
	_cp_my_debug(c_code)
	f_out.write(c_code)
	_cp_my_close_file(f_out)
	_cp_my_debug("$$$$$$$$$$$$$$$$$$$$$$$$")
	_cp_my_debug(MM_CODE)


def cp_parse_ctags(filename):
	""" cp_parse_ctags
	# Parses the ctags file into memory, stores it in lol (list of lists)
	# Arguments:
	#	filename	output from ctags run
	# Result:
	#	Data structure lol is populated, each item in lol contains 1 line from
	#	the ctags file
	"""
	# local list of lists 
	lol = []
	# open file for reading 
	f_ctags = _cp_my_open_file(filename)
	
	### parses each line of ctags file
	for line in f_ctags:
		tmp = line.split(None, 4)
		tmp = cp_cleanup(tmp)
		lol.append(tmp) 
   
	### close file
	_cp_my_close_file(f_ctags)
	return lol


def main(filename, original_f) :
	""" main
	#	Controls the program execution
	# Arguments:
	#	filename	name of ctags output file
	#	original_f	original user C code
	# Result:
	#	compilable C code for RPC client and RPC server
	"""
	### lol (list of lists) contains each elemet I need to deal with 
	lol = []
	### parses the ctags file and populates lol
	lol = cp_parse_ctags(filename)   
	### writes out c code using each element in lol
	cp_write_c(lol, filename, original_f)


### standard main for python
if __name__ == "__main__":
	if len(sys.argv) == 5:
		FILE_NAME = sys.argv[1]
		ORIGINAL_FILE = sys.argv[2]
		MM_C_OUT = sys.argv[3]
		MM_H_OUT = sys.argv[4]
		main(FILE_NAME, ORIGINAL_FILE)

	else :
		_cp_my_debug("Usage: ./ctags_parser.py <tags_file> <original_file>")
		sys.exit()

