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
#define SET_ERR_AND_RETURN(x)			\
int result = 1;					\
result = x;					\
 if (result < 0) {				\
 __set_errno(-result);				\
 return -1;					\
 }						\
 return result					

/* This file is auto-generated! do not edit it manually!*/

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