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
OUT_PATH = "./output/"


def _cp_my_debug (message) :
	""" cp_my_debug --
	#	Prints message to console
	# Arguments:
	#	message	text to be printed to console
	# Result: 
	#	message is printed to the console
	"""
	print message


def _cp_my_open_file(target, mode = "r"):
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


def cp_middle_magic(sig) :
	""" cp_middle_magic
	# Generates the code/function to connect client and server.
	# Constructs a function with the arguments that need to be send
	# Arguments:
	#	sig	function signature as provided by ctags
	# Result:
	# 	middle_string	C code to call function in my library with the right
	#		number of arguments to be sent accross the network
	"""
	sig = sig # pylint likes this better than not using the var
	return ""


def cp_ret_lookup(ret_t) :
	""" cp_ret_lookup
	#	Actually does the work determining what should actually be returned
	#	based on the signatures return type the 
	# Arguments:
	#	ret_t	first OR second word of the function signature (int bla()=>int)
	# Result:
	#	value that allows for compilable C code to be generated
	"""
	### TODO figure out what actually should be returned -- coordinate with 
	###	cp_return_type. Import it and put in different class
	if "int" in ret_t or "char" in ret_t or "long" in ret_t:
		return str(0)+";"
	if "void" in ret_t:
		return ";"
	else : 
		### TODO: THIS IS A HACK. Fix it!!!
		return str(0) + ";"


def cp_return_type(signature) :
	""" cp_return_type
	# Determines the return type and a return value. Later this will be
	#	changed to return the result of the server-call
	# Arguments:
	#	signature	function signature as delivered by ctags
	# Result:
	#	string of the form "return <value>;"
	"""
	### TODO figure out what actually should be returned
	
	ret = ""
	tmp = signature.split()
	if "extern" in tmp[0] or "signed" in tmp[0]:
		ret += "return " + cp_ret_lookup(tmp[1])
	else :
		ret += "return " + cp_ret_lookup(tmp[0])
	return ret


def cp_function_middle(signature) :
	""" cp_function_middle
	# Deals with the middle of the function. A function should look like this:
	#	<signature> {
	#	[<connection magic>]
	#	return <client-server-call()>;
	#	}
	# Arguments:
	#	signature	signature of the function as delivered by ctags
	# Result:
	#	body of the function, including the return statement (i.e the part
	#	between the '{' '}'
	"""
	### TODO will have to deal with the connection magic
	middle = ""
	return_statement = ""
	middle += cp_middle_magic(signature)
	# return statement returns int so that it compiles
	# later we return middle_magic();
	return_statement += cp_return_type(signature)
	middle += return_statement
	middle = middle # pylint likes this better than not using the var
	return return_statement
	# return middle


def cp_get_struct(lol, i) :
	""" cp_get_struct
	# Gets full lol and pulls out the struct statring at i. Intended to deal
	#	with multi-line structs, tyedef structs, etc.
	# Arguments:
	#	lol	list of lists. Contains line by line representation of ctags
	#	i	current location in lol, starting point of struct
	# Result:
	#	Compilable struct and C code built from ctags information
	"""
	ret_string = ""
	ret_string += lol[i][-1] + "{\n"
	### struct starts at i and stops 1 line after the members end
	while lol[i+1][TYPE] == "member" :
		i = i + 1
		if "," in str(lol[i][-1]) and "[" in str(lol[i][-1]):
			ret_string += "/* Look into this and FIX ME: line "
			ret_string +=  str(lol[i][LINE_NUM]) + "*/ "
			ret_string +=  str(lol[i][SIGNATURE]) + ";\n"
			line_num = lol[i][LINE_NUM]
			while line_num == lol[i][LINE_NUM]:
				i = i + 1
		
		### TODO deal with const 
		### TODO nested structs 
		if "signed" in str(lol[i][-1].split()[0]):
			ret_string +=  str(lol[i][-1].split()[0]) 
			ret_string += " " + str(lol[i][-1].split()[1])+" " 
			ret_string += lol[i][SYM_NAME]+";\n"
		elif "," not in str(lol[i][-1]):
			ret_string += str(lol[i][SIGNATURE]) + ";\n"
		else :
			ret_string += str(lol[i][-1].split()[0]) 
			ret_string += " " + str(lol[i][SYM_NAME]) + ";\n"
	if lol[i+1][TYPE] == "typedef" :
		ret_string += lol[i+1][-1]+";\n"
	else :
		ret_string += "};\n"
	return ret_string


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
	#	includes of the format #include <foo> (not #include "foo")
	# Arguments:
	#	original_c_file	C code from user that uses standard library
	# Result:
	#	string of includes: #include <foo.h>
	"""
	ret_s = ""
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


def cp_parse_ctags(filename):
	""" cp_parse_ctags
	# Parses the ctags file into memory, stores it in lol (list of lists)
	# Arguments:
	#	filename	output from ctags run
	# Result:
	#	Data structure lol is populated, each item in lol contains 1 line from
	#	the ctags file
	"""
	### local list of lists
	lol = []
	### open file for reading 
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
	#	Result:
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
	if len(sys.argv) == 3:
		FILE_NAME = sys.argv[1]
		ORIGINAL_FILE = sys.argv[2]
		main(FILE_NAME, ORIGINAL_FILE)
	else :
		_cp_my_debug("Usage: ./ctags_parser.py <tags_file> <original_file>")
		sys.exit()

