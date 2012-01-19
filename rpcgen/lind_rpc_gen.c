
/* This file is auto-generated! do not edit it manually!*/

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "lind_rpc.h"
#include "lind_syscalls.h"
#include "strace.h"
#include "nacl_util.h"
#include "nacl_syscalls.h"
#include "component.h"
#include <nacl_stat.h>
#include <sys/statfs.h>
#include <sys/stat.h>
int lind_access_rpc(int version, const char * file);
int lind_unlink_rpc(const char * name);
int lind_link_rpc(const char * from, const char * to);
int lind_chdir_rpc(const char * name);
int lind_mkdir_rpc(int mode, const char * path);
int lind_rmdir_rpc(const char * path);
int lind_xstat_rpc(int version, const char * path, struct stat * buf);
int lind_open_rpc(int fd, int mode, const char * path);
int lind_close_rpc(int fd);
int lind_read_rpc(int fd, int size, void * buf);
int lind_write_rpc(int desc, size_t count, const void * buf);
int lind_lseek_rpc(off_t offset, int fd, int whence, off_t* ret);
int lind_fxstat_rpc(int fd, int version, struct stat * buf);
int lind_fstatfs_rpc(int fd, struct statfs * buf);
int lind_statfs_rpc(const char * path, struct statfs * buf);
int lind_noop_rpc(void);
int lind_getpid_rpc(pid_t * buf);
int lind_dup_rpc(int oldfd);
int lind_dup2_rpc(int oldfd, int newfd);
int lind_getdents_rpc(int fd, size_t nbytes, char * buf);
int lind_fcntl_get_rpc(int fd, int cmd);
int lind_fcntl_set_rpc(int fd, int cmd, long set_op);
int lind_socket_rpc(int domain, int type, int protocol);
int lind_bind_rpc(int sockfd, socklen_t addrlen, __CONST_SOCKADDR_ARG addr);
struct lind_access_rpc_s {
	int version;
};

struct lind_unlink_rpc_s {
};

struct lind_link_rpc_s {
};

struct lind_chdir_rpc_s {
};

struct lind_mkdir_rpc_s {
	int mode;
};

struct lind_rmdir_rpc_s {
};

struct lind_xstat_rpc_s {
	int version;
};

struct lind_open_rpc_s {
	int fd;
	int mode;
};

struct lind_close_rpc_s {
	int fd;
};

struct lind_read_rpc_s {
	int fd;
	int size;
};

struct lind_write_rpc_s {
	int desc;
	size_t count;
};

struct lind_lseek_rpc_s {
	off_t offset;
	int fd;
	int whence;
};

struct lind_fxstat_rpc_s {
	int fd;
	int version;
};

struct lind_fstatfs_rpc_s {
	int fd;
};

struct lind_statfs_rpc_s {
};

struct lind_noop_rpc_s {
};

struct lind_getpid_rpc_s {
};

struct lind_dup_rpc_s {
	int oldfd;
};

struct lind_dup2_rpc_s {
	int oldfd;
	int newfd;
};

struct lind_getdents_rpc_s {
	int fd;
	size_t nbytes;
};

struct lind_fcntl_get_rpc_s {
	int fd;
	int cmd;
};

struct lind_fcntl_set_rpc_s {
	int fd;
	int cmd;
	long set_op;
};

struct lind_socket_rpc_s {
	int domain;
	int type;
	int protocol;
};

struct lind_bind_rpc_s {
	int sockfd;
	socklen_t addrlen;
};

/* This file is auto-generated! do not edit it manually!*/

int lind_access_rpc(int version, const char * file) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_access_rpc_s args;
memset(&args, 0, sizeof(struct lind_access_rpc_s));
int return_code = -1;
args.version = version;
size_t file_len = strlen(file);
size_t file_size = file_len;
const char * file_len_str = nacl_itoa(file_size);
request.format = combine(4, "", "<i" , file_len_str, "s");
request.call_number = 2;
free((void*)file_len_str);
request.message.len = sizeof(struct lind_access_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 1, file, file_size);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_unlink_rpc(const char * name) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_unlink_rpc_s args;
memset(&args, 0, sizeof(struct lind_unlink_rpc_s));
int return_code = -1;
size_t name_len = strlen(name);
size_t name_size = name_len;
const char * name_len_str = nacl_itoa(name_size);
request.format = combine(3, "", name_len_str, "s");
request.call_number = 4;
free((void*)name_len_str);
request.message.len = sizeof(struct lind_unlink_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 1, name, name_size);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_link_rpc(const char * from, const char * to) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_link_rpc_s args;
memset(&args, 0, sizeof(struct lind_link_rpc_s));
int return_code = -1;
size_t from_len = strlen(from);
size_t from_size = from_len;
const char * from_len_str = nacl_itoa(from_size);
size_t to_len = strlen(to);
size_t to_size = to_len;
const char * to_len_str = nacl_itoa(to_size);
request.format = combine(5, "", from_len_str, "s", to_len_str, "s");
request.call_number = 5;
free((void*)from_len_str);
free((void*)to_len_str);
request.message.len = sizeof(struct lind_link_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 2, from, from_size, to, to_size);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_chdir_rpc(const char * name) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_chdir_rpc_s args;
memset(&args, 0, sizeof(struct lind_chdir_rpc_s));
int return_code = -1;
size_t name_len = strlen(name);
size_t name_size = name_len;
const char * name_len_str = nacl_itoa(name_size);
request.format = combine(3, "", name_len_str, "s");
request.call_number = 6;
free((void*)name_len_str);
request.message.len = sizeof(struct lind_chdir_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 1, name, name_size);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_mkdir_rpc(int mode, const char * path) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_mkdir_rpc_s args;
memset(&args, 0, sizeof(struct lind_mkdir_rpc_s));
int return_code = -1;
args.mode = mode;
size_t path_len = strlen(path);
size_t path_size = path_len;
const char * path_len_str = nacl_itoa(path_size);
request.format = combine(4, "", "<i" , path_len_str, "s");
request.call_number = 7;
free((void*)path_len_str);
request.message.len = sizeof(struct lind_mkdir_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 1, path, path_size);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_rmdir_rpc(const char * path) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_rmdir_rpc_s args;
memset(&args, 0, sizeof(struct lind_rmdir_rpc_s));
int return_code = -1;
size_t path_len = strlen(path);
size_t path_size = path_len;
const char * path_len_str = nacl_itoa(path_size);
request.format = combine(3, "", path_len_str, "s");
request.call_number = 8;
free((void*)path_len_str);
request.message.len = sizeof(struct lind_rmdir_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 1, path, path_size);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_xstat_rpc(int version, const char * path, struct stat * buf) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_xstat_rpc_s args;
memset(&args, 0, sizeof(struct lind_xstat_rpc_s));
int return_code = -1;
args.version = version;
size_t path_len = strlen(path);
size_t path_size = path_len;
const char * path_len_str = nacl_itoa(path_size);
request.format = combine(4, "", "<i" , path_len_str, "s");
request.call_number = 9;
free((void*)path_len_str);
request.message.len = sizeof(struct lind_xstat_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 1, path, path_size);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
assert( CONTENTS_SIZ(reply) <= sizeof(struct stat));
memcpy(buf, reply.contents, CONTENTS_SIZ(reply));
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_open_rpc(int fd, int mode, const char * path) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_open_rpc_s args;
memset(&args, 0, sizeof(struct lind_open_rpc_s));
int return_code = -1;
args.fd = fd;
args.mode = mode;
size_t path_len = strlen(path);
size_t path_size = path_len;
const char * path_len_str = nacl_itoa(path_size);
request.format = combine(5, "", "<i" , "<i" , path_len_str, "s");
request.call_number = 10;
free((void*)path_len_str);
request.message.len = sizeof(struct lind_open_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 1, path, path_size);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_close_rpc(int fd) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_close_rpc_s args;
memset(&args, 0, sizeof(struct lind_close_rpc_s));
int return_code = -1;
args.fd = fd;
request.format = """<i" ;
request.call_number = 11;
request.message.len = sizeof(struct lind_close_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_read_rpc(int fd, int size, void * buf) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_read_rpc_s args;
memset(&args, 0, sizeof(struct lind_read_rpc_s));
int return_code = -1;
args.fd = fd;
args.size = size;
request.format = """<i" "<i" ;
request.call_number = 12;
request.message.len = sizeof(struct lind_read_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
assert( CONTENTS_SIZ(reply) <= size);
memcpy(buf, reply.contents, CONTENTS_SIZ(reply));
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_write_rpc(int desc, size_t count, const void * buf) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_write_rpc_s args;
memset(&args, 0, sizeof(struct lind_write_rpc_s));
int return_code = -1;
args.desc = desc;
args.count = count;
const char * count_len_str = nacl_itoa(count);
request.format = combine(5, "", "<i" , "<I" , count_len_str, "s");
request.call_number = 13;
free((void*)count_len_str);
request.message.len = sizeof(struct lind_write_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 1, buf, count);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_lseek_rpc(off_t offset, int fd, int whence, off_t* ret) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_lseek_rpc_s args;
memset(&args, 0, sizeof(struct lind_lseek_rpc_s));
int return_code = -1;
args.offset = offset;
args.fd = fd;
args.whence = whence;
request.format = """<q" "<i" "<i" ;
request.call_number = 14;
request.message.len = sizeof(struct lind_lseek_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
assert( CONTENTS_SIZ(reply) <= sizeof(off_t));
memcpy(ret, reply.contents, CONTENTS_SIZ(reply));
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_fxstat_rpc(int fd, int version, struct stat * buf) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_fxstat_rpc_s args;
memset(&args, 0, sizeof(struct lind_fxstat_rpc_s));
int return_code = -1;
args.fd = fd;
args.version = version;
request.format = """<i" "<i" ;
request.call_number = 17;
request.message.len = sizeof(struct lind_fxstat_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
assert( CONTENTS_SIZ(reply) <= sizeof(struct stat));
memcpy(buf, reply.contents, CONTENTS_SIZ(reply));
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_fstatfs_rpc(int fd, struct statfs * buf) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_fstatfs_rpc_s args;
memset(&args, 0, sizeof(struct lind_fstatfs_rpc_s));
int return_code = -1;
args.fd = fd;
request.format = """<i" ;
request.call_number = 19;
request.message.len = sizeof(struct lind_fstatfs_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
assert( CONTENTS_SIZ(reply) <= sizeof(struct statfs));
memcpy(buf, reply.contents, CONTENTS_SIZ(reply));
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_statfs_rpc(const char * path, struct statfs * buf) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_statfs_rpc_s args;
memset(&args, 0, sizeof(struct lind_statfs_rpc_s));
int return_code = -1;
size_t path_len = strlen(path);
size_t path_size = path_len;
const char * path_len_str = nacl_itoa(path_size);
request.format = combine(3, "", path_len_str, "s");
request.call_number = 26;
free((void*)path_len_str);
request.message.len = sizeof(struct lind_statfs_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 1, path, path_size);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
assert( CONTENTS_SIZ(reply) <= sizeof(struct statfs));
memcpy(buf, reply.contents, CONTENTS_SIZ(reply));
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_noop_rpc(void) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_noop_rpc_s args;
memset(&args, 0, sizeof(struct lind_noop_rpc_s));
int return_code = -1;
request.format = "";
request.call_number = 1;
request.message.len = sizeof(struct lind_noop_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_getpid_rpc(pid_t * buf) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_getpid_rpc_s args;
memset(&args, 0, sizeof(struct lind_getpid_rpc_s));
int return_code = -1;
request.format = "";
request.call_number = 31;
request.message.len = sizeof(struct lind_getpid_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
assert( CONTENTS_SIZ(reply) <= sizeof(pid_t));
memcpy(buf, reply.contents, CONTENTS_SIZ(reply));
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_dup_rpc(int oldfd) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_dup_rpc_s args;
memset(&args, 0, sizeof(struct lind_dup_rpc_s));
int return_code = -1;
args.oldfd = oldfd;
request.format = """<i" ;
request.call_number = 24;
request.message.len = sizeof(struct lind_dup_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_dup2_rpc(int oldfd, int newfd) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_dup2_rpc_s args;
memset(&args, 0, sizeof(struct lind_dup2_rpc_s));
int return_code = -1;
args.oldfd = oldfd;
args.newfd = newfd;
request.format = """<i" "<i" ;
request.call_number = 25;
request.message.len = sizeof(struct lind_dup2_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_getdents_rpc(int fd, size_t nbytes, char * buf) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_getdents_rpc_s args;
memset(&args, 0, sizeof(struct lind_getdents_rpc_s));
int return_code = -1;
args.fd = fd;
args.nbytes = nbytes;
request.format = """<i" "<I" ;
request.call_number = 23;
request.message.len = sizeof(struct lind_getdents_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
assert( CONTENTS_SIZ(reply) <= nbytes);
memcpy(buf, reply.contents, CONTENTS_SIZ(reply));
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_fcntl_get_rpc(int fd, int cmd) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_fcntl_get_rpc_s args;
memset(&args, 0, sizeof(struct lind_fcntl_get_rpc_s));
int return_code = -1;
args.fd = fd;
args.cmd = cmd;
request.format = """<i" "<i" ;
request.call_number = 28;
request.message.len = sizeof(struct lind_fcntl_get_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_fcntl_set_rpc(int fd, int cmd, long set_op) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_fcntl_set_rpc_s args;
memset(&args, 0, sizeof(struct lind_fcntl_set_rpc_s));
int return_code = -1;
args.fd = fd;
args.cmd = cmd;
args.set_op = set_op;
request.format = """<i" "<i" "<q" ;
request.call_number = 28;
request.message.len = sizeof(struct lind_fcntl_set_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_socket_rpc(int domain, int type, int protocol) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_socket_rpc_s args;
memset(&args, 0, sizeof(struct lind_socket_rpc_s));
int return_code = -1;
args.domain = domain;
args.type = type;
args.protocol = protocol;
request.format = """<i" "<i" "<i" ;
request.call_number = 32;
request.message.len = sizeof(struct lind_socket_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 0);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}
/* This file is auto-generated! do not edit it manually!*/

int lind_bind_rpc(int sockfd, socklen_t addrlen, __CONST_SOCKADDR_ARG addr) {
lind_request request;
memset(&request, 0, sizeof(request));
lind_reply reply;
memset(&reply, 0, sizeof(reply));
struct lind_bind_rpc_s args;
memset(&args, 0, sizeof(struct lind_bind_rpc_s));
int return_code = -1;
args.sockfd = sockfd;
args.addrlen = addrlen;
const char * addrlen_len_str = nacl_itoa(addrlen);
request.format = combine(5, "", "<i" , "<i" , addrlen_len_str, "s");
request.call_number = 33;
free((void*)addrlen_len_str);
request.message.len = sizeof(struct lind_bind_rpc_s);
request.message.body = &args;
nacl_rpc_syscall_proxy(&request, &reply, 1, addr, addrlen);
/* on error return negative so we can set ERRNO. */
if (reply.is_error) {
return_code = reply.return_code * -1;
} else {
return_code = reply.return_code;
}
return return_code;
}