#!/usr/bin/python


import sys
import os

SYM_NAME = 0
TYPE = 1
SOURCE_FILE = 2
SIGNATURE = 3
OUT_PATH = "./output/"
c_file = ""

### prints stuff to console
def cp_my_debug (s) :
	print s

### opens files with argument
def cp_my_open_file(f, mode = "r"):
	return open(f, mode)

### closes file
def cp_my_close_file(f):
	f.close()




### removes the unneeded colums from ctags
def cp_cleanup(s) :
	del s[2]
	s[-1] = s[-1].split(";")[0]
	if s[-1][-1] == '\n':
		s[-1] = s[-1][:-1]
	
	return s



### actually does the work determining what should actually be returned
### TODO figure out what actually should be returned -- coordinate with 
###	cp_return_type
def cp_ret_lookup(ret_t) :
	if "int" in ret_t or "char" in ret_t or "long" in ret_t:
		return str(0)+";"
	if "void" in ret_t:
		return ";"



### determines the return type and plugs in a value
### TODO figure out what actually should be returned
def cp_return_type(signature) :
	ret = ""
	tmp = signature.split()
	if "extern" in tmp[0] or "signed" in tmp[0]:
		ret = ret + "return " + cp_ret_lookup(tmp[1])
	else :
		ret = ret + "return " + cp_ret_lookup(tmp[0])
	return ret


### deals with the middle of the function
### TODO will have to deal with the connection magic
def cp_function_middle(signature) :
	middle = ""
	tmp = signature.split()

	return_statement = cp_return_type(signature)
	middle = middle + return_statement
	return middle


### gets full lol and pulls out the struct statring at i
def cp_get_struct(lol, i) :
	ret_string = ""
	ret_string = ret_string +lol[i][-1] + "{\n"
	### struct starts at i and stops 1 line after the members end
	while lol[i+1][TYPE] == "member" :
		i = i + 1
		ret_string = ret_string +  str(lol[i][-1].split()[0]) 
		### TODO deal with const 
		### TODO nested structs
		if "signed" in str(lol[i][-1].split()[0]):
			ret_string = ret_string +" " + str(lol[i][-1].split()[1])+" " + lol[i][SYM_NAME]+";\n"
	
	ret_string = ret_string +lol[i+1][-1]+";\n"
	return ret_string


### inputs typedefs into the string
def cp_get_typedef(signature) :
	tmp = ""
	tmp = tmp + signature +";\n"
	return tmp 


### function signature 	
def cp_get_fstring(signature) :
	function = str(signature)+" {\n"
	function = function + cp_function_middle(signature)
	return function+"\n}\n"



### write c code
def cp_write_c(lol, filename) :
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
	f = cp_my_open_file(OUT_PATH+filename+".andi", "w+")
	cp_my_debug(c_code)
	f.write(c_code)
	cp_my_close_file(f)



### parses the ctags file (stores element 0,1,4)
def cp_parse_ctags(filename):
	### local list of lists
	lol = []
	### open file for reading 
	f = cp_my_open_file(filename)
	
	### parses each line of ctags file
	for line in f:
		tmp = line.split(None,4)
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

