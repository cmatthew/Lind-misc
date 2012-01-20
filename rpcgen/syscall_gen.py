# this is for things which go in the header file (lind_rpc_gen.h)
header_file = []

# things which are to go at the top of the C file
top = []

# headers needed for the rpc to work. Will be put in both the C and H file
headers = ["#include <stddef.h>",
           "#include <stdlib.h>",
           "#include <string.h>",
           "#include <unistd.h>",
           "#include <assert.h>",
           "#include <sys/types.h>",
           "#include <sys/socket.h>",
           "#include \"lind_rpc.h\"",
           "#include \"lind_syscalls.h\"",
           "#include \"strace.h\"",
           "#include \"nacl_util.h\"",
           "#include \"nacl_syscalls.h\"",
           "#include \"component.h\"",
           "#include <nacl_stat.h>",
           "#include <sys/statfs.h>",
           "#include <sys/stat.h>"
           ]

# All the rpc structs
structs = [ ]

# global vars
globals = [ ]

# function bodies are stored in here
output = [ ]

# syscall handlers all set error no. and return in a similar way,
# so include this macro to do this cleanly
auto_return = ["\n#define SET_ERR_AND_RETURN(x)			\\",
"int result = 1;					\\",
"result = x;					\\",
" if (result < 0) {				\\",
" __set_errno(-result);				\\",
" return -1;					\\",
" }						\\",
" return result					",
""]
header_file.extend(auto_return)


# don't you hate it when you edit a file which get over written in the
# build?  Me too! So warn who ever is editing this file that it will
# be blow away.
warning = "/* This file is auto-generated! do not edit it manually!*/\n"

# now add to both the header and the C file
top.append(warning)
header_file.append(warning)

# We need format strings to describe the data as it is passed
# set those up here, first simple types, then complex types

#the primitive format types: signed and unsigned int and long
fint = "<i"
fuint = "<I"
flong = "<q"
fulong = "<Q"

# this table maps between system types and the format strings used to
# represent them.  Format strings are the RePy Struct module format
# strings. 
fmt_map = { "int":fint,
            "unsigned int":fuint,
            "long":flong,
            "unsigned long":fulong,
            "off_t":flong, #check this! it might be in error
            "unsigned long int":fulong,
            "mode_t":fint,
            "size_t":fuint,
            "ssize_t":fint,
            "socklen_t":fint
            }


def struct_def(name, types):
    """ Given a call name, make the matching struct
    with all the struct fields defined in the tuples
    passed in the types list.

    NAME: The RPC functions name (to create struct function_name_s).
    TYPES: A list of the types to go in this struct, each type is a tuple
    which is a (type, identifier) pair.

    """
    body = ["struct ", name,"_s {\n" ]
    for i in xrange(0,len(types)):
        body.append("\t")
        body.append(types[i][0])
        body.append(" ")
        body.append(types[i][1])
        body.append(';\n')
    body.append("};\n")
    return ''.join(body)



def func_decl(name, types):
    """ Given a call name, make a function declairation
    with all the parameters defined in the tuples
    passed in the types list.

    NAME: The RPC functions name.
    TYPES: A list of the types to go into the parameter list, each type is a tuple
    which is a (type, identifier) pair.

    """

    body = ["int ", name, '(']
    if not types:
        body.append("void")
    else:
        for i in xrange(0,len(types)):
            body.append(types[i][0])
            body.append(" ")
            body.append(types[i][1])
            body.append(', ')
        body = body[:-1]
    body.append(")")
    return ''.join(body)



def build_format_string(in_args, ref_args):
    """Build the RPC description string for a function given its arguments
    IN_ARGS: a (type name) tuple list for each of the pass by value parameters
    REF_ARGS: a (type name size) tuple list for each parameter which should be
    passed by reference
    RETURNS: a tuple (pre, fmt, post, send)
    PRE: is operations to do before the send ( to make the format string)
    FMT: is the format string command which will end up being a simple set of
    strings or a \"combine\" command to dynamically build the string.
    POST: is cleanup operations to do after the string is built
    SEND: is a reference parameter list with sizes which is needed by the
    proxy send command, which is based off the format string, so it is built
    here as well.
    """
    fmt = ["\"\""]
    pre = []
    post = []
    send_args = []

    # make simple pass by value args
    # these just have to be a sting like "<i"
    for arg in in_args:
        fmt.append("\"" + fmt_map[arg[0]] + "\" ")

    # Now do reference args, find the size and make a string 
    if ref_args:
        for args in ref_args:
            # strings do have size, so build them with strlen
            if is_string(args[0]):
                name = args[1]

                #build format string number from strlen result
                pre.append("size_t "+ name +"_len = strlen("+name+");")
                pre.append("size_t "+ name+"_size = "+name+"_len;") 
                pre.append("const char * "+name+"_len_str = nacl_itoa("+name+"_size);")

                # add to the reference args list along with the send size
                send_args.append(name)
                send_args.append(name+"_size")

                # clean up the strings after
                post.append("free((void*)"+name+"_len_str);")

                # finally, make the actual format string
                fmt.append(name+"_len_str")
                fmt.append("\"s\"")
            else:
                # since we are not a string, take the third parameter which is a
                # size and use that.
                name = args[2]
                pre.append("const char * "+name+"_len_str = nacl_itoa("+name+");")
                post.append("free((void*)"+name+"_len_str);")

                send_args.append(args[1])
                send_args.append(name)

                fmt.append(name+"_len_str")
                fmt.append("\"s\"")
        # dynamically combine reference args using string join
        fmt = ["combine(", str(len(fmt)),", ",', '.join(fmt),")"]
                
    return pre,''.join(fmt),post,send_args


def is_string(type_arg):
     return type_arg == "char *" or type_arg =="const char *"

def syscall(name, in_args, ref_args=[], out_args=[]):
    """
    Build a system call rpc call.

    Concatenats many strings to build them.

    NAME: the syscalls name  (note should be in the syscall table above already!)

    a TYPE_LIST is a list of tuples in the format (type str, identifier name str)
 
    IN_ARGS: a TYPE_LIST of the pass by value parameters

    REF_ARGS: a TYPE_LIST of the pass by reference parameters.  Each tuple whoes type is not a
    string must have a third tuple member which specifies the size of the argument.  This can
    be any valid C expression like the name of another idetifier or a sizeof(foo).

    OUT_ARGS: a TYPE_LIST of the output parameters. Note, each parameter may have a third tuple
    element with a size check parameter which the actual output is checked to be smaller than.
    The size check can be anthing that fits in an if like sizeof(something) or just a another
    identifier.

    RETURNS: nothing. output is in sideeffects

    SIDEEFFECTS: Adds lines of code to the global output lists
   
    """

    # the actual call name is lind_SYSCALL_rpc
    call_name = 'lind_'+name+'_rpc'

    # make struct for call by value args to be packed in
    structs.append( struct_def(call_name, in_args ))

    # the function parameter list has all the args, so combine them here
    all_args= in_args + ref_args + out_args

    # build the header file function declairation
    header_file.append(func_decl(call_name, all_args)+';')

    # add one to the top of the file too
    headers.append(func_decl(call_name, all_args)+';')

    # add extra warning message that the code is auto generated
    # for anyone who greped to get here and missed the top of the file
    output.append(warning)

    # function definition
    output.append(func_decl(call_name, all_args)+' {')

    # set up the request and reply buffers
    output.extend(["lind_request request;",
                   "memset(&request, 0, sizeof(request));",
                   "lind_reply reply;",
                   "memset(&reply, 0, sizeof(reply));",

                   "struct lind_" + name + "_rpc_s args;",
                   "memset(&args, 0, sizeof(struct lind_" + name + "_rpc_s));",
                   "int return_code = -1;"])

    # for each value argument, add to the message struct
    for args in in_args:
        output.append("args."+args[1] + " = " + args[1] + ";")
        # output.append("nacl_strace(nacl_itoa(sizeof("+args[1]+")));")

    # get the format string and its builders
    pre,fmt,post,tosend = build_format_string(in_args, ref_args)

    # build the format string
    output.extend(pre)

    #assign the final format string
    output.append("request.format = " + fmt + ";")

    # enable these for protocol debugging:
    # output.append("nacl_strace(request.format);")
    # output.append("nacl_strace(nacl_itoa(sizeof(struct "+call_name+"_s)));")

    # assign the syscall number from the table
    output.append("request.call_number = " + str(syscall_table[name][0]) + ";")

    # add cleanup commands
    output.extend(post)

    # attach value args to the rpc payload
    output.append("request.message.len = sizeof(struct " + call_name + "_s);")
    output.append("request.message.body = &args;")

    # count the number of reference arguments
    # and add name, len pairs to the reference list
    if tosend:
        vargs = str(len(tosend)/2) + ", " + ', '.join(tosend) 
    else:
        # if no reference args, then append 0
        vargs = "0"

    # make the actual RPC call
    output.append("nacl_rpc_syscall_proxy(&request, &reply, "+vargs+");")

    # now deal with the returned informatoin
    check_ret_code = ["/* on error return negative so we can set ERRNO. */",
                      "if (reply.is_error) {",
                      "return_code = reply.return_code * -1;",
                      "} else {",
                      "return_code = reply.return_code;",]
    output.extend(check_ret_code)

    # if there is an out arg, check its size and copy it into the target
    if out_args:
        # size check arguement is present
        if len(out_args[0]) == 3:
            output.append("assert( CONTENTS_SIZ(reply) <= "+out_args[0][2]+");")
        # copy from the RPC payload to the target pointer
        output.append("memcpy(" + out_args[0][1] + ", reply.contents, CONTENTS_SIZ(reply));")

    # finally return and close function body
    output.extend(["}",
                  "return return_code;",
                   "}"])




# Now start setting up the system calls



# we need to know which number each syscall takes in the dispatch table
syscall_table = {
    "noop":(1, "lind_debug_noop","lind_debug_noop"),
    "access":(2, "lind_fs_access","lind_fs_safe_access"),
    "trace":(3,),
    "unlink":(4,),
    "link":(5,),
    "chdir":(6,),
    "mkdir":(7,),
    "rmdir":(8,),
    "xstat":(9, "lind_fs_xstat","lind_fs_safe_xstat"),
    "open":(10, "lind_fs_open","lind_fs_safe_open"),
    "close":(11, "lind_fs_close","lind_fs_safe_close"),
    "read":(12, "lind_fs_read","lind_fs_safe_read"),
    "write":(13, "lind_fs_write","lind_fs_safe_write"),
    "lseek":(14, "lind_fs_lseek","lind_fs_safe_lseek"),
    "ioctl":(15,),
    "fxstat":(17,),
    "fstatfs":(19,),
    "getdents":(23,),
    "dup":(24,),
    "dup2":(25,),
    "statfs":(26,),
    # these both are recieved by the same handler on the other side
    # so they can have the same syscall number
    "fcntl_get":(28,),
    "fcntl_set":(28,),
    "getpid":(31,),
    "socket":(32,),
    "bind":(33,),
 
 
    }


#
# Now define each system call
#

syscall("access", [("int","version")],[("const char *","file")])
syscall("unlink",[],[("const char *","name")])
syscall("link",[],[("const char *","from"),("const char *","to")])
syscall("chdir",[],[("const char *","name")])
syscall("mkdir",[("int","mode")],[("const char *", "path")])
syscall("rmdir",[],[("const char *","path")])
syscall("xstat", [("int","version")],[("const char *","path")], [("struct stat *", "buf", "sizeof(struct stat)")])
syscall("open", [("int","fd"), ("int","mode")],[("const char *","path")])
syscall("close",[("int","fd")])
syscall("read", [("int","fd"), ("int","size")],[], [("void *", "buf", "size")])
syscall("write",[("int","desc"), ("size_t", "count")],[("const void *", "buf","count")])
#syscall("lseek", [("int","fd"),("int","whence")],[],[("off_t*","ret", "sizeof(off_t)")])
# note the strange order here, this is because our struct module does not deal with byte alignment
syscall("lseek", [("off_t","offset"),("int","fd"),("int","whence")],[],[("off_t*","ret", "sizeof(off_t)")])
syscall("fxstat", [("int","fd"),("int","version")],[], [("struct stat *", "buf", "sizeof(struct stat)")])
syscall("fstatfs", [("int","fd")],[], [("struct statfs *", "buf", "sizeof(struct statfs)")])
syscall("statfs", [],[("const char *","path")], [("struct statfs *", "buf", "sizeof(struct statfs)")])
syscall("noop", [],[])
syscall("getpid", [],[], [("pid_t *", "buf", "sizeof(pid_t)")])
syscall("dup",[("int","oldfd")])
syscall("dup2",[("int","oldfd"),("int","newfd")])
syscall("getdents",[("int","fd"),("size_t","nbytes")],[],[("char *","buf","nbytes")])
syscall("fcntl_get",[("int","fd"),("int","cmd")])
syscall("fcntl_set",[("int","fd"),("int","cmd"),("long","set_op")])
syscall("socket",[("int","domain"),("int","type"),("int","protocol")])
syscall("bind",[("int","sockfd"),("socklen_t","addrlen")],[("__CONST_SOCKADDR_ARG","addr","addrlen")])






# Now write out the system calls to the screen the src files to be compiled.

hfd = open('lind_rpc_gen.h', 'w')
c_file = open('lind_rpc_gen.c','w')

hfd.write( '\n'.join(headers) )
c_file.write('\n')

hfd.write('\n'.join(header_file))

hfd.close()

c_file.write('\n'.join(top))
c_file.write('\n')

c_file.write('\n'.join(headers))
c_file.write('\n')

c_file.write('\n'.join(structs))
c_file.write('\n')

c_file.write('\n'.join(output))

c_file.close()

# also print to the screen for debugging

print "/*Header:*/"
print '\n'.join(headers)

print '\n'.join(header_file)


print "\n/*Code*/\n"
print '\n'.join(top)
print
print '\n'.join(headers)
print
print '\n'.join(structs)
print 
print '\n'.join(output)

