#!/usr/bin/python


import sys
import os

SYM_NAME = 0
TYPE = 1
LINE_NUM = 2
SOURCE_FILE = 3
SIGNATURE = 4
OUT_PATH = "./output/"
c_file = ""

def cp_my_debug (message) :
	"""Prints message to console"""
	print message


def cp_my_open_file(target, mode = "r"):
	"""Opens target file"""
	return open(target, mode)


def cp_my_close_file(target):
	"""Closes target file"""
	target.close()




def cp_cleanup(strings) :
	"""Removes the unneeded colums from ctags"""
#	del strings[2]
	strings[-1] = strings[-1].split(";")[0]
	if strings[-1][-1] == '\n':
		strings[-1] = strings[-1][:-1]
	
	return strings


def cp_ret_lookup(ret_t) :
	""" actually does the work determining what should actually be returned """
	### TODO figure out what actually should be returned -- coordinate with 
	###	cp_return_type. Import it and put in different class
	if "int" in ret_t or "char" in ret_t or "long" in ret_t:
		return str(0)+";"
	if "void" in ret_t:
		return ";"



def cp_return_type(signature) :
	""" determines the return type and plugs in a value"""
	### TODO figure out what actually should be returned
	
	ret = ""
	tmp = signature.split()
	if "extern" in tmp[0] or "signed" in tmp[0]:
		ret = ret + "return " + cp_ret_lookup(tmp[1])
	else :
		ret = ret + "return " + cp_ret_lookup(tmp[0])
	return ret



def cp_function_middle(signature) :
	""" deals with the middle of the function"""
	### TODO will have to deal with the connection magic
	middle = ""
	tmp = signature.split()

	return_statement = cp_return_type(signature)
	middle = middle + return_statement
	return middle



def cp_get_struct(lol, i) :
	""" gets full lol and pulls out the struct statring at i"""
	ret_string = ""
	ret_string = ret_string +lol[i][-1] + "{\n"
	### struct starts at i and stops 1 line after the members end
	while lol[i+1][TYPE] == "member" :
		i = i + 1
		if "," in str(lol[i][-1]) and "[" in str(lol[i][-1]):
			ret_string = ret_string +"/* Look into this and FIXME: line "
			ret_string = ret_string + str(lol[i][LINE_NUM]) + "*/ "
			ret_string = ret_string + str(lol[i][SIGNATURE]) + ";\n"
			line_num = lol[i][LINE_NUM]
			while line_num == lol[i][LINE_NUM]:
				i = i + 1
		
		### TODO deal with const 
		### TODO nested structs 
		if "signed" in str(lol[i][-1].split()[0]):
			ret_string = ret_string +  str(lol[i][-1].split()[0]) 
			ret_string = ret_string +" " + str(lol[i][-1].split()[1])+" " 
			ret_string = ret_string + lol[i][SYM_NAME]+";\n"
		elif "," not in str(lol[i][-1]):
			ret_string = ret_string + str(lol[i][SIGNATURE]) + ";\n"
		else :
			ret_string = ret_string +  str(lol[i][-1].split()[0]) 
			ret_string = ret_string + " " + str(lol[i][SYM_NAME]) + ";\n"
	if lol[i+1][TYPE] == "typedef" :
		ret_string = ret_string +lol[i+1][-1]+";\n"
	else :
		ret_string = ret_string + "};\n"
	return ret_string



def cp_get_typedef(signature) :
	""" inputs typedefs into the string"""
	tmp = ""
	tmp = tmp + signature +";\n"
	return tmp 



def cp_get_fstring(signature) :
	"""function signature is returned"""
	function = str(signature)+" {\n"
	function = function + cp_function_middle(signature)
	return function+"\n}\n"



def cp_write_c(lol, filename) :
	""" write c code"""
	c_code = ""
	
	### deals with typedefs that are not structs
	member = False
	for i in range(len(lol)) :
		if lol[i][TYPE] == "typedef" and lol[i-1][TYPE] != "member":
			c_code = c_code + cp_get_typedef(lol[i][SIGNATURE])
	### deals with structs
	for i in range(len(lol)):
		if lol[i][TYPE] == "struct":
			c_code = c_code + cp_get_struct(lol, i)
	### deals with prototypes
	for item in lol :
		if item[TYPE] == "prototype":
			c_code = c_code + cp_get_fstring(item[SIGNATURE])
	os.system("touch "+OUT_PATH+filename+".andi")
	f_out = cp_my_open_file(OUT_PATH+filename+".andi", "w+")
	cp_my_debug(c_code)
	f_out.write(c_code)
	cp_my_close_file(f_out)



def cp_parse_ctags(filename):
	""" parses the ctags file into memory """
	### local list of lists
	lol = []
	### open file for reading 
	f = cp_my_open_file(filename)
	
	### parses each line of ctags file
	for line in f:
		tmp = line.split(None, 4)
		tmp = cp_cleanup(tmp)
		lol.append(tmp) 
   
	### close file
	cp_my_close_file(f)
	return lol



### starts execution of the program
def main(filename) :
	### lol (list of lists) contains each elemet I need to deal with 
	lol = []
	### parses the ctags file and populates lol
	lol = cp_parse_ctags(filename)   
	### writes out c code using each element in lol
	cp_write_c(lol, filename)



### standard main for python
if __name__ == "__main__":
	if len(sys.argv) > 1:
		filename = sys.argv[1]
		main(filename)
	else :
		cp_my_debug("no input file")
		sys.exit()

