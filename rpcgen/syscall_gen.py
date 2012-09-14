"""
Build the Lind RPC stubs.

This program generates the C rpc stubs for all the Lind system calls.

It replaces the more error prone and less flexible manual approach.

Works by filling the output lists with lines of code, then dumping
them to output files at the end.


"""

import sys
from string import Template

def uniqueify(seq):
    seen = set()
    seen_add = seen.add
    return [x for x in seq if x not in seen and not seen_add(x)]


# this is for things which go in the header file (lind_rpc_gen.h)
_header_file = []

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
           "#include <sys/select.h>",
           "#include <sys/poll.h>",
           "#include \"lind_rpc.h\"",
           "#include \"lind_syscalls.h\"",
           "#include \"strace.h\"",
           "#include \"nacl_util.h\"",
           "#include \"nacl_syscalls.h\"",
           "#include \"component.h\"",
           "#include <nacl_stat.h>",
           "#include <sys/statfs.h>",
           "#include <sys/stat.h>"]

# All the rpc structs
structs = []

# function bodies are stored in here
output = []

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
_header_file.extend(auto_return)


# don't you hate it when you edit a file which get over written in the
# build?  Me too! So warn who ever is editing this file that it will
# be blow away.
WARNING_MSG = "/* This file is auto-generated! do not edit it manually!*/\n"

# now add to both the header and the C file
top.append(WARNING_MSG)
_header_file.append(WARNING_MSG)

# We need format strings to describe the data as it is passed
# set those up here, first simple types, then complex types

#the primitive format types: signed and unsigned int and long
FINT = "<i"
FUINT = "<I"
FLONG = "<q"
FULONG = "<Q"

# this table maps between system types and the format strings used to
# represent them.  Format strings are the RePy Struct module format
# strings.

fmt_map = {"int": FINT,
           "unsigned int": FUINT,
           "long": FINT,
           "long long": FLONG,
           "unsigned long": FUINT,
           "off_t": FLONG, #check this! it might be in error
           "unsigned long int": FUINT,
           "mode_t": FINT,
           "size_t": FUINT,
           "ssize_t": FINT,
           "socklen_t": FINT}


def struct_def(name, types):
    """ Given a call name, make the matching struct
    with all the struct fields defined in the tuples
    passed in the types list.

    NAME: The RPC functions name (to create struct function_name_s).
    TYPES: A list of the types to go in this struct, each type is a tuple
    which is a (type, identifier) pair.

    """
    body = ["struct ", name, "_s {\n"]
    for i in xrange(0, len(types)):
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
    TYPES: A list of the types to go into the parameter list,
    each type is a tuple
    which is a (type, identifier) pair.

    """

    body = ["int ", name, '(']
    if not types:
        body.append("void")
    else:
        for i in xrange(0, len(types)):
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

    first_ref_arg = True
    # make simple pass by value args
    # these just have to be a sting like "<i"
    for arg in in_args:
        fmt.append("\"" + fmt_map[arg[0]] + "\" ")
    # Now do reference args, find the size and make a string
    if ref_args:
        for args in ref_args:
            name = args[1]

            # Null support, pass P if name is null
            null_check = "// check if ${name} is NULL\n" \
                         "int ${name}_is_null = 0;\n" \
                         "if (${name} == NULL) {\n" \
                         "${name}_is_null = 1;\n" \
                         "} else {\n" \
                         "${name}_is_null = 0;\n" \
                         "}"
            pre.append(Template(null_check).substitute(name=name))

            
            # strings do have size, so build them with strlen          
            # since we are not a string, take the third parameter which is
            # a size and use that.
            if first_ref_arg:
                pre.append("unsigned long int nowhere = 0;")
                first_ref_arg = False

            var_name = args[1]
            try:
                size_expr = args[2]
            except IndexError:
                if is_string(args[0]):
                    size_expr = "strlen(" + var_name + ")"
                else:
                    msg = ("Could not find size of argument while building " +
                           "parameter " +
                           str(args[1]))
                    print msg
                    sys.exit(1)




            size_expr_ssize = var_name + "_ssize"
            format_specifier = var_name + "_fmt_str"
            data_pointer = var_name + "_ptr"
            final_size = var_name + "_data_size"
            # sometimes our size_vars have non-alpha characters in them
            # get rid of those!
            format_size_var = str(filter(lambda x: x.isalnum(), size_expr)) + "_"  + var_name + "_len_str"



            pre.append("ssize_t " + size_expr_ssize + " = " + size_expr +  ";")
            pre.append("const char * " + format_size_var + ' = "YYY";')

            pre.append('const char * ' + format_specifier + ' = "ZZZ";')

            pre.append("void * " + var_name + "_ptr = NULL;")
            pre.append("ssize_t " + final_size + " = -1;")


            pre.append("if (" + var_name + "_is_null) {")
            pre.append(data_pointer + " = &nowhere;")
            pre.append(format_specifier + ' =  "P";')
            pre.append(format_size_var + ' = "";')
            pre.append(final_size + " = sizeof (void *);")
            pre.append("} else {")
            pre.append(data_pointer + " = (void*)"+ var_name  +";")
            pre.append(format_specifier + ' =  "s"; // if it is not null, treat as a string')
            pre.append(format_size_var + " =  nacl_itoa("+size_expr_ssize+");")
            pre.append(final_size + " = " + size_expr_ssize + ";"   )           
            pre.append("}")

            pre.append("assert( "+data_pointer+" != NULL );")
            pre.append("assert( " + final_size + " != -1 );")

            pre.append("assert( strcmp(" + format_size_var + ', "YYY") != 0 );')
            pre.append("assert( strcmp(" + format_specifier + ', "ZZZ") != 0 );')

            post.append("if (! " + var_name  + "_is_null) {")
            post.append("free((void*)" + format_size_var + ");")
            post.append("}")
            send_args.append(data_pointer)
            send_args.append(final_size)

            fmt.append(format_size_var)
            fmt.append(format_specifier)

        
        # dynamically combine reference args using string join
        fmt = ["combine(", str(len(fmt)), ", ", ', '.join(fmt), ")"]
    return pre, ''.join(fmt), post, send_args


def is_string(type_arg):
    """is the type string in type_arg a C string?
    Really should check for all combinations of char *
    but for now, lets just check the simple ones.
    """
    tpye_arg = type_arg.strip()
    return type_arg == "char *" or type_arg == "const char *"


def determine_reply_size(out_args):
    """Given an out arugments list,
    figure out the optimal size of the return buffer.
    """
    # find the size of each type
    type_sizes = []
    for types in out_args:
        if len(types) <= 2:
            type_sizes.append("sizeof(" + types[0] + ")")
        else:
            type_sizes.append(types[2])
    if not type_sizes:
        type_sizes = ["0"]

    #make room for the reply header
    type_sizes.append("sizeof(lind_reply)")

    # now make the total
    size_stmt = "size_t reply_size = " + " + ".join(type_sizes) + ";"
    return (size_stmt, "reply_size")


def syscall(name, in_args, ref_args=[], out_args=[]):
    """
    Build a system call rpc call.

    Concatenats many strings to build them.

    NAME: the syscalls name  (note should be in the syscall table above
    already!)

    a TYPE_LIST is a list of tuples in the format (type str, identifier
    name str)

    IN_ARGS: a TYPE_LIST of the pass by value parameters

    REF_ARGS: a TYPE_LIST of the pass by reference parameters.  Each tuple
    whoes type is not a string must have a third tuple member which specifies
    the size of the argument.  This can be any valid C expression like the
    name of another idetifier or a sizeof(foo).

    OUT_ARGS: a TYPE_LIST of the output parameters. Note, each parameter
    may have a third tuple element with a size check parameter which the
    actual output is checked to be smaller than. The size check can be
    anthing that fits in an if like sizeof(something) or just a another
    identifier.

    RETURNS: nothing. output is in sideeffects

    SIDEEFFECTS: Adds lines of code to the global output lists
   """

    # the actual call name is lind_SYSCALL_rpc
    call_name = 'lind_' + name + '_rpc'

    # make struct for call by value args to be packed in
    structs.append(struct_def(call_name, in_args))

    # the function parameter list has all the args, so combine them here
    all_args = in_args + ref_args + out_args

    # build the header file function declairation
    _header_file.append(func_decl(call_name, all_args) + ';')

    # add one to the top of the file too
    headers.append(func_decl(call_name, all_args) + ';')

    # add extra WARNING_MSG message that the code is auto generated
    # for anyone who greped to get here and missed the top of the file
    output.append(WARNING_MSG)

    # function definition
    output.append(func_decl(call_name, all_args) + ' {')

    # set up the request and reply buffers
    (setup, reply_size_expr) = determine_reply_size(out_args)
    output.extend(["lind_request request;",
                   "memset(&request, 0, sizeof(request));",
                   "int return_code = -1;",
                   setup,
#                   "nacl_strace(\"" + call_name + "\");
#                   nacl_strace(nacl_itoa(reply_size));",
                   "lind_reply* reply_buffer = malloc(" + \
                   reply_size_expr + ");",
                   "assert(reply_buffer!=NULL);",
                   "/* These memsets are mainly for saftey.*/",
                   "memset(reply_buffer, 0, " + reply_size_expr  \
                   + ");",

                   "struct lind_" + name + "_rpc_s args;",
                   "memset(&args, 0, sizeof(struct lind_" + name + "_rpc_s));",
                   ])
    output.append("nacl_strace(\"" + call_name + "\\n\");")

    # for each value argument, add to the message struct
    for args in in_args:
        output.append("args." + args[1] + " = " + args[1] + ";")
        # output.append("nacl_strace(nacl_itoa(sizeof("+args[1]+")));")

    # get the format string and its builders
    pre, fmt, post, tosend = build_format_string(in_args, ref_args)

    # build the format string
    output.extend(pre)

    #assign the final format string
    output.append("request.format = " + fmt + ";")

    # enable these for protocol debugging:
    # output.append('nacl_strace("Request.Format:");')
    # output.append("nacl_strace(request.format);")
    # output.append("nacl_strace(\""+call_name+"\");")

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
        vargs = str(len(tosend) / 2) + ", " + ', '.join(tosend)
    else:
        # if no reference args, then append 0
        vargs = "0"
    # make the actual RPC call
    # output.append("nacl_strace(request.format);")
    output.append("nacl_rpc_syscall_proxy(&request, reply_buffer,"
                  + vargs + ");")

    # now deal with the returned informatoin
    check_ret_code = ["/* on error return negative so we can set ERRNO. */",
                      "if (reply_buffer->is_error) {",
                      "return_code = reply_buffer->return_code * -1;",
                      "} else {",
                      "return_code = reply_buffer->return_code;"]
    output.extend(check_ret_code)

    # if there is an out arg, check its size and copy it into the target
    if out_args:
        complex = True if len(out_args) > 1 else False
        # first check sizes multi argument check, size should be
        # EXACTLY equal to the sum of the parts must be exactly equal
        # becuase we could not know which one was too small otherwise
        if complex:
            pass
        # for single arugment, it can be smaller than the buffer, just
        # not larger.
        else:
            output.append("assert( CONTENTS_SIZ(reply_buffer) <= " +
                          out_args[0][2] + ");")
        # this will hold each output argument.
        output.append("void * ptr = NULL;")
        old_sizes = []
        if not complex:
            for arg in out_args:
                # copy from the RPC payload to the target pointer
                old_sizes_str = "+ " + \
                (" + ".join(old_sizes) if old_sizes else "0")
                output.append("ptr = " + arg[1] + ";")
                output.append("if (ptr != NULL) { memcpy(ptr, ((&(reply_buffer->contents)) " + \
                              old_sizes_str + "), " \
                              + arg[2] + "); }")
                old_sizes.append(arg[2])
        else: # complex args
            # header has sizes
            output.append("void * base = &(reply_buffer->contents);")

            output.append("int * sizes = base;")
            output.append("int offset=(sizeof(int) * " + str(len(out_args)) \
                          + "), cur_size = 0;\n")

            for arg in enumerate(out_args):
                # copy from the RPC payload to the target pointer
                old_sizes_str = "+ " + \
                                (" + ".join(old_sizes) if old_sizes else "0")
                output.append("ptr = (void *)" + arg[1][1] + ";")
                output.append("cur_size = sizes[" + str(arg[0]) + "];")
                #output.append("nacl_strace(nacl_itoa(cur_size));")
                #output.append("nacl_strace(nacl_itoa(offset));")

                #output.append("nacl_strace( base + offset);")
                output.append("if (ptr != NULL) {")
                output.append("assert( cur_size <= " + str(arg[1][2]) + " );")
                output.append("memcpy(ptr, base +  offset, cur_size);")
                output.append("} // end of ptr != NULL")
                output.append("offset += cur_size;\n")

    # finally return and close function body
    output.extend(["}",
                   "free(reply_buffer);",
                   "return return_code;",
                   "}"])


def emptysyscall(name, in_args, ref_args=[], out_args=[]):
    """
    Sometimes we just want an empty system call stub to be
    generated.  This is nomrally when we dont have the real call
    working yet.  Works the same as system_call, but makes an empty
    C stub.
    """

    # the actual call name is lind_SYSCALL_rpc
    call_name = 'lind_' + name + '_rpc'

    # the function parameter list has all the args, so combine them here
    all_args = in_args + ref_args + out_args

    # build the header file function declairation
    _header_file.append(func_decl(call_name, all_args) + ';')

    # add one to the top of the file too
    headers.append(func_decl(call_name, all_args) + ';')

    # add extra WARNING_MSG message that the code is auto generated
    # for anyone who greped to get here and missed the top of the file
    output.append(WARNING_MSG)

    # function definition
    output.append(func_decl(call_name, all_args) + ' {')
    output.append('return 0; }')



# Now start setting up the system calls



# we need to know which number each syscall takes in the dispatch table
# ideally we could generate repy side too, so record that info as well
syscall_table = {
    "noop": (1, "lind_debug_noop", "lind_debug_noop"),
    "access": (2, "lind_fs_access", "lind_fs_safe_access"),
    "trace": (3,),
    "unlink": (4,),
    "link": (5,),
    "chdir": (6,),
    "mkdir": (7,),
    "rmdir": (8,),
    "xstat": (9, "lind_fs_xstat", "lind_fs_safe_xstat"),
    "open": (10, "lind_fs_open", "lind_fs_safe_open"),
    "close": (11, "lind_fs_close", "lind_fs_safe_close"),
    "read": (12, "lind_fs_read", "lind_fs_safe_read"),
    "write": (13, "lind_fs_write", "lind_fs_safe_write"),
    "lseek": (14, "lind_fs_lseek", "lind_fs_safe_lseek"),
    "ioctl": (15,),
    "fxstat": (17,),
    "fstatfs": (19,),
    "getdents": (23,),
    "dup": (24,),
    "dup2": (25,),
    "statfs": (26,),
    # these both are recieved by the same handler on the other side
    # so they can have the same syscall number
    "fcntl_get": (28,),
    "fcntl_set": (28,),
    "getpid": (31,),
    "socket": (32,),
    "bind": (33,),
    "send": (34,),
    "sendto": (35,),
    "recv": (36,),
    "recvfrom": (37,),
    "connect": (38,),
    "listen": (39,),
    "accept": (40,),
    "getpeername": (41,),
    "getsockname": (42,),
    "getsockopt": (43,),
    "setsockopt": (44,),
    "select": (46,),
    "getifaddrs": (47,),
    "poll": (48,),
    "socketpair": (49,),
    "getuid": (50,),
    "geteuid": (51,),
    "getgid": (52,),
    "getegid": (53,),
    "flock":(54,),
   "shutdown": (45,),}


#
# Now define each system call
#

syscall("access", [("int", "version")], [("const char *", "file")])

syscall("unlink", [], [("const char *", "name")])

syscall("link", [], [("const char *", "from"), ("const char *", "to")])

syscall("chdir", [], [("const char *", "name")])

syscall("mkdir", [("int", "mode")], [("const char *", "path")])

syscall("rmdir", [], [("const char *", "path")])

syscall("xstat", [("int", "version")],
                 [("const char *", "path")],
                 [("struct stat *", "buf", "sizeof(struct stat)")])

syscall("open", [("int", "fd"), ("int", "mode")], [("const char *", "path")])

syscall("close", [("int", "fd")])

syscall("read", [("int", "fd"), ("int", "size")],
                [],
                [("void *", "buf", "size")])

syscall("write", [("int", "desc"),
                  ("size_t", "count")],
                 [("const void *", "buf", "count")])

# note the strange order here, this is because our struct module does
# not deal with byte alignment

syscall("lseek", [("off_t", "offset"),
                  ("int", "fd"),
                  ("int", "whence")],
                 [],
                 [("off_t*", "ret", "sizeof(off_t)")])

syscall("fxstat", [("int", "fd"),
                   ("int", "version")],
                  [],
                  [("struct stat *", "buf", "sizeof(struct stat)")])

syscall("fstatfs", [("int", "fd")],
                   [],
                   [("struct statfs *", "buf", "sizeof(struct statfs)")])

syscall("statfs", [],
                  [("const char *", "path")],
                  [("struct statfs *", "buf", "sizeof(struct statfs)")])

syscall("noop", [], [])

syscall("getpid", [], [], [("pid_t *", "buf", "sizeof(pid_t)")])

syscall("dup", [("int", "oldfd")])

syscall("dup2", [("int", "oldfd"), ("int", "newfd")])

syscall("getdents", [("int", "fd"), ("size_t", "nbytes")],
                    [],
                    [("char *", "buf", "nbytes")])

syscall("fcntl_get", [("int", "fd"), ("int", "cmd")])

syscall("fcntl_set", [("int", "fd"), ("int", "cmd"), ("long", "set_op")])

syscall("socket", [("int", "domain"), ("int", "type"), ("int", "protocol")])

syscall("bind", [("int", "sockfd"), ("socklen_t", "addrlen")],
                [("const struct sockaddr *", "addr", "addrlen")])

syscall("send", [("int", "sockfd"), ("size_t", "len"), ("int", "flags")],
                [("const void *", "buf", "len")])


syscall("recv", [("int", "sockfd"),
                 ("size_t", "len"),
                 ("int", "flags")],
                [],
                [("void *", "buf", "len")])

syscall("connect", [("int", "sockfd"),
                    ("socklen_t", "addrlen")],
                   [("const struct sockaddr *", "src_addr", "addrlen")])

syscall("listen", [("int", "sockfd"), ("int", "backlog")])


emptysyscall("sendto", [("int", "sockfd"),
                        ("size_t", "len"),
                        ("int", "flags"),
                        ("socklen_t", "addrlen")],
                       [("const struct sockaddr_in *", "dest_addr", "addrlen"),
                        ("const void *", "buf", "len")])


syscall("accept", [("int", "sockfd"), ("socklen_t", "addrlen")],
 [],[]) #[("__SOCKADDR_ARG","addr_out","addrlen"), ("socklen_t*", "addrlen_out", "sizeof(socklen_t)")])

emptysyscall("getpeername",
             [("int", "sockfd"),
              ("socklen_t", "addrlen_in")],
             [],
             [("__SOCKADDR_ARG", "addr", "addrlen_in"),
              ("socklen_t *", "addrlen_out", "sizeof(socklen_t)",
               "addr")])

# # getpeername needs in out type too!
# # "getsockname"
# "getsockopt",
syscall("setsockopt", [("int", "sockfd"),
                            ("int", "level"),
                            ("int", "optname"),
                            ("socklen_t", "optlen")],
                           [("const void *", "optval", "optlen")])

syscall("getsockopt", [("int", "sockfd"),
                            ("int", "level"),
                            ("int", "optname"),
                            ("socklen_t", "optlen")], [],
                           [("void *", "optval", "optlen")])


syscall("shutdown", [("int", "sockfd"), ("int", "how")])


# Use have_* to deal with case when they are null.  When not null,
# make the have_* = to sizeof(fd_set)
syscall("select", [("int", "nfds")],
                  [("fd_set *", "readfds", "sizeof(fd_set)"),
                   ("fd_set *", "writefds", "sizeof(fd_set)"),
                   ("fd_set *", "exceptfds", "sizeof(fd_set)"),
                   ("struct timeval *", "timeout",
                    "sizeof(struct timeval)"), ],
                  [("struct select_results *", "result",
                    "sizeof(struct select_results)")])

syscall("getifaddrs", [("int", "ifaddrs_buf_siz"), ], [],
                           [("void *", "ifaddrs", "ifaddrs_buf_siz")])


syscall("recvfrom", [("int", "sockfd"),
                     ("size_t", "len"),
                     ("int", "flags"),
                     ("socklen_t", "addrlen")],
                    [],
                    [("socklen_t*", "addrlen_out", "sizeof(socklen_t)"),
                     ("void *", "buf", "len"),
                     ("struct sockaddr *", "src_addr",
                      "sizeof(struct sockaddr)")])


syscall("poll", [("int", "nfds"),("int","timeout")],
        [("struct pollfd *", "fds_in", "(sizeof(struct pollfd) * nfds)")],
        [("struct pollfd *", "fds_out", "(sizeof(struct pollfd) * nfds)")])


syscall("socketpair", [("int", "domain"), ("int", "type"), ("int", "protocol")], [], [("int *", "fds", "sizeof(int) * 2")])

syscall("getuid", [],[],[("uid_t *", "buf", "sizeof(uid_t)")])
syscall("geteuid", [],[],[("uid_t *", "buf", "sizeof(uid_t)")])
syscall("getgid", [],[],[("gid_t *", "buf", "sizeof(gid_t)")])
syscall("getegid", [],[],[("gid_t *", "buf", "sizeof(gid_t)")])

syscall("flock", [("int", "fd"), ("int", "operation")],[],[])


# Now write out the system calls to the screen the src files to be compiled.

hfd = open('lind_rpc_gen.h', 'w')
c_file = open('lind_rpc_gen.c', 'w')

hfd.write('\n'.join(headers))
c_file.write('\n')

hfd.write('\n'.join(_header_file))

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


indent_level = 0

def pretty_print(blocks):
    """prefix tabs on code based on { and }
    Does not handle comments etc, just 
    """
    global indent_level
    for lines in blocks:
        for line in lines.split('\n'):
            if '}' in line:
                indent_level -= 1
            yield '\t'*indent_level + line
            if '{' in line:
                indent_level += 1
                    

print "/*Header:*/"
print '\n'.join(headers)

print '\n'.join(_header_file)

print "\n/*Code*/\n"
print '\n'.join(pretty_print(top))
print
print '\n'.join(pretty_print(headers))
print
print '\n'.join(pretty_print(structs))
print
print '\n'.join(pretty_print(output))
