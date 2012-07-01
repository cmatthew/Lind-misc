#!/usr/bin/env python

#    Copyright (C) 2001  Jeff Epler  <jepler@unpythonic.dhs.org>
#    Copyright (C) 2006  Csaba Henk  <csaba.henk@creo.hu>
#
#    This program can be distributed under the terms of the GNU LGPL.
#    See the file COPYING.
#

import sys
import os
import stat 
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
import lind_test_server as lind
# and now back to where we started.
os.chdir(cur)

execfile("/home/lind/tmp/lind/repy/dispatcher.repy")

from errno import *
from stat import *
import fcntl
# pull in some spaghetti to make this stuff work without fuse-py being installed
try:
    import _find_fuse_parts
except ImportError:
    pass
import fuse
from fuse import Fuse
import posix

errno = {
    'EPERM':1,	# Operation not permitted 
    'ENOENT':2, # No such file or directory
    'ESRCH': 3,	# No such process 
    'EINTR': 4,	# Interrupted system call 
    'EIO': 5,	# I/O error 
    'ENXIO': 6,	# No such device or address 
    'E2BIG': 7,	# Argument list too long 
    'ENOEXEC': 8,	# Exec format error 
    'EBADF': 9,	# Bad file number 
    'ECHILD':10,	# No child processes 
    'EAGAIN':11,	# Try again 
    'ENOMEM':12,	# Out of memory 
    'EACCES':13,	# Permission denied 
    'EFAULT':14,	# Bad address 
    'ENOTBLK':15,	# Block device required 
    'EBUSY':16,	# Device or resource busy 
    'EEXIST':17,	# File exists 
    'EXDEV':18,	# Cross-device link 
    'ENODEV':19,	# No such device 
    'ENOTDIR':20,	# Not a directory 
    'EISDIR':21,	# Is a directory 
    'EINVAL':22,	# Invalid argument 
    'ENFILE':23,	# File table overflow 
    'EMFILE':24,	# Too many open files 
    'ENOTTY':25,	# Not a typewriter 
    'ETXTBSY':26,	# Text file busy 
    'EFBIG':27,	# File too large 
    'ENOSPC':28,	# No space left on device 
    'ESPIPE':29,	# Illegal seek 
    'EROFS':30,	# Read-only file system 
    'EMLINK':31,	# Too many links 
    'EPIPE':32,	# Broken pipe 
    'EDOM':33,	# Math argument out of domain of func 
    'ERANGE':34,	# Math result not representable 

    'EDEADLK':35,	# Resource deadlock would occur 
    'ENAMETOOLONG':36,	# File name too long 
    'ENOLCK':37,  # No record locks available 
    'ENOSYS':38,	# Function not implemented 
    'ENOTEMPTY':39,	# Directory not empty 
    'ELOOP':40,	# Too many symbolic links encountered 
#    'EWOULDBLOCK':EAGAIN,	# Operation would block 
    'ENOMSG':42,	# No message of desired type 
    'EIDRM':43,	# Identifier removed 
    'ECHRNG':44,	# Channel number out of range 
    'EL2NSYNC':45,	# Level 2 not synchronized 
    'EL3HLT':46,	# Level 3 halted 
    'EL3RST':47,	# Level 3 reset 
    'ELNRNG':48,	# Link number out of range 
    'EUNATCH':49,	# Protocol driver not attached 
    'ENOCSI':50,	# No CSI structure available 
    'EL2HLT':51,	# Level 2 halted 
    'EBADE':52,	# Invalid exchange 
    'EBADR':53,	# Invalid request descriptor 
    'EXFULL':54,	# Exchange full 
    'ENOANO':55,	# No anode 
    'EBADRQC':56,	# Invalid request code 
    'EBADSLT':57,	# Invalid slot 
    'EBFONT':59,	# Bad font file format 
    'ENOSTR':60,	# Device not a stream 
    'ENODATA':61,	# No data available 
    'ETIME':62,	# Timer expired 
    'ENOSR':63,	# Out of streams resources 
    'ENONET':64,	# Machine is not on the network 
    'ENOPKG':65,	# Package not installed 
    'EREMOTE':66,	# Object is remote 
    'ENOLINK':67,	# Link has been severed 
    'EADV':68,	# Advertise error 
    'ESRMNT':69,	# Srmount error 
    'ECOMM':70,	# Communication error on send 
    'EPROTO':71,	# Protocol error 
    'EMULTIHOP':72,	# Multihop attempted 
    'EDOTDOT':73,	# RFS specific error 
    'EBADMSG':74,	# Not a data message 
    'EOVERFLOW':75,	# Value too large for defined data type 
    'ENOTUNIQ':76,	# Name not unique on network 
    'EBADFD':77,	# File descriptor in bad state 
    'EREMCHG':78,	# Remote address changed 
    'ELIBACC':79,	# Can not access a needed shared library 
    'ELIBBAD':80,	# Accessing a corrupted shared library 
    'ELIBSCN':81,	# .lib section in a.out corrupted 
    'ELIBMAX':82,	# Attempting to link in too many shared libraries 
    'ELIBEXEC':83,	# Cannot exec a shared library directly 
    'EILSEQ':84,	# Illegal byte sequence 
    'ERESTART':85,	# Interrupted system call should be restarted 
    'ESTRPIPE':86,	# Streams pipe error 
    'EUSERS':87,	# Too many users 
    'ENOTSOCK':88,	# Socket operation on non-socket 
    'EDESTADDRREQ':89,	# Destination address required 
    'EMSGSIZE':90,	# Message too long 
    'EPROTOTYPE':91,	# Protocol wrong type for socket 
    'ENOPROTOOPT':92,	# Protocol not available 
    'EPROTONOSUPPORT':93,	# Protocol not supported 
    'ESOCKTNOSUPPORT':94,	# Socket type not supported 
    'EOPNOTSUPP':95,	# Operation not supported on transport endpoint 
    'EPFNOSUPPORT':96,	# Protocol family not supported 
    'EAFNOSUPPORT':97,	# Address family not supported by protocol 
    'EADDRINUSE':98,	# Address already in use 
    'EADDRNOTAVAIL':99,	# Cannot assign requested address 
    'ENETDOWN':100,	# Network is down 
    'ENETUNREACH':101,	# Network is unreachable 
    'ENETRESET':102,	# Network dropped connection because of reset 
    'ECONNABORTED':103,	# Software caused connection abort 
    'ECONNRESET':104,	# Connection reset by peer 
    'ENOBUFS':105,	# No buffer space available 
    'EISCONN':106,	# Transport endpoint is already connected 
    'ENOTCONN':107,	# Transport endpoint is not connected 
    'ESHUTDOWN':108,	# Cannot send after transport endpoint shutdown 
    'ETOOMANYREFS':109,	# Too many references: cannot splice 
    'ETIMEDOUT':110,	# Connection timed out 
    'ECONNREFUSED':111,	# Connection refused 
    'EHOSTDOWN':112,	# Host is down 
    'EHOSTUNREACH':113,	# No route to host 
    'EALREADY':114,	# Operation already in progress 
    'EINPROGRESS':115,	# Operation now in progress 
    'ESTALE':116,	# Stale NFS file handle 
    'EUCLEAN':117,	# Structure needs cleaning 
    'ENOTNAM':118,	# Not a XENIX named type file 
    'ENAVAIL':119,	# No XENIX semaphores available 
    'EISNAM':120,	# Is a named type file 
    'EREMOTEIO':121,	# Remote I/O error 
    'EDQUOT':122,	# Quota exceeded 
    'ENOMEDIUM':123,	# No medium found 
    'EMEDIUMTYPE':124,	# Wrong medium type 
    'ECANCELED':125,	# Operation Canceled 
    'ENOKEY':126,	# Required key not available 
    'EKEYEXPIRED':127,	# Key has expired 
    'EKEYREVOKED':128,	# Key has been revoked 
    'EKEYREJECTED':129,	# Key was rejected by service 
    # for robust mutexes 
    'EOWNERDEAD':130,	# Owner died 
    'ENOTRECOVERABLE':131	# State not recoverable
    }

if not hasattr(fuse, '__version__'):
    raise RuntimeError, \
        "your fuse-py doesn't know of fuse.__version__, probably it's too old."

fuse.fuse_python_api = (0, 2)

fuse.feature_assert('stateful_files', 'has_init')


def flag2mode(flags):
    md = {os.O_RDONLY: 'r', os.O_WRONLY: 'w', os.O_RDWR: 'w+'}
    m = md[flags & (os.O_RDONLY | os.O_WRONLY | os.O_RDWR)]

    if flags | os.O_APPEND:
        m = m.replace('w', 'a', 1)

    return m


class MyStat(fuse.Stat):
    def __init__(self):
        self.st_mode = 0
        self.st_ino = 0
        self.st_dev = 0
        self.st_nlink = 0
        self.st_uid = 0
        self.st_gid = 0
        self.st_size = 0
        self.st_atime = 0
        self.st_mtime = 0
        self.st_ctime = 0
        

class Xmp(Fuse):

    def __init__(self, *args, **kw):

        Fuse.__init__(self, *args, **kw)

        # do stuff to set up your filesystem here, if you want
        #import thread
        #thread.start_new_thread(self.mythread, ())
        self.root = '/'

#    def mythread(self):
#
#        """
#        The beauty of the FUSE python implementation is that with the python interp
#        running in foreground, you can have threads
#        """
#        print "mythread: started"
#        while 1:
#            time.sleep(120)
#            print "mythread: ticking"

    def getattr(self, path):
#        print "getattr", path
        try:
            stats = lind.stat_syscall(path)
        except lind.SyscallError, e:
            return -errno[e[1]]
        devid, inode, mode, linkcount, uid, gid, rdev,size, blocksize, blocks, \
               atime, atimens, mtime, mtimens, ctime, ctimens = stats
        st = MyStat()
        # if path == '/':
        #     st.st_mode = stat.S_IFDIR | 0755
        #     st.st_nlink = 2
        # else:
        st.st_mode = mode # stat.S_IFREG | 0444
        st.st_nlink = linkcount
        st.st_size = size
        st.st_dev = devid
        st.st_uid = uid
        st.st_gid = gid
        st.st_ino = inode
        st.st_atime = atime
        st.st_ctime = ctime
        st.st_mtime = mtime
 
        return st


    def readlink(self, path):
        print "readlink", path
        return os.readlink("." + path)


    def readdir(self, path, offset):
        
        lindfd = lind.open_syscall(path, lind.O_RDONLY, lind.S_IRWXU)
        dents = map(lambda x:x[1], lind.getdents_syscall(lindfd, 999))
                        
        assert len(dents) < 998, "Readdir max was hit..."
        for e in dents:
            yield fuse.Direntry(e)


    def unlink(self, path):
        print "unlink"


    def rmdir(self, path):
#        print "rmdir", path
        try:
            ret = lind.rmdir_syscall(path)
        except lind.SyscallError, e:
            ret = -errno[e[1]]
        return ret
        

    def symlink(self, path, path1):
        print "symlink", path


    def rename(self, path, path1):
        print "rename", path


    def link(self, path, path1):
        print "link", path


    def chmod(self, path, mode):
        print "chmod", path


    def chown(self, path, user, group):
        print "chown", path


    def truncate(self, path, len):
        print "truncate", path


    def mknod(self, path, mode, dev):
        print "mknod"


    def mkdir(self, path, mode):
 #       print "mkdir", path
        try:
            ret = lind.mkdir_syscall(path, mode)
        except lind.SyscallError, e:
            ret = -errno[e[1]]
        return ret

    def utime(self, path, times):
        print "utime"

#    The following utimens method would do the same as the above utime method.
#    We can't make it better though as the Python stdlib doesn't know of
#    subsecond preciseness in acces/modify times.
#  
#    def utimens(self, path, ts_acc, ts_mod):
#      os.utime("." + path, (ts_acc.tv_sec, ts_mod.tv_sec))

    def access(self, path, mode):
        print "access", path
        ret = lind.access_syscall(path, mode)
        return ret
        
#    This is how we could add stub extended attribute handlers...
#    (We can't have ones which aptly delegate requests to the underlying fs
#    because Python lacks a standard xattr interface.)
#
#    def getxattr(self, path, name, size):
#        val = name.swapcase() + '@' + path
#        if size == 0:
#            # We are asked for size of the value.
#            return len(val)
#        return val
#
#    def listxattr(self, path, size):
#        # We use the "user" namespace to please XFS utils
#        aa = ["user." + a for a in ("foo", "bar")]
#        if size == 0:
#            # We are asked for size of the attr list, ie. joint size of attrs
#            # plus null separators.
#            return len("".join(aa)) + len(aa)
#        return aa

    def statfs(self):
        """
        Should return an object with statvfs attributes (f_bsize, f_frsize...).
        Eg., the return value of os.statvfs() is such a thing (since py 2.2).
        If you are not reusing an existing statvfs object, start with
        fuse.StatVFS(), and define the attributes.

        To provide usable information (ie., you want sensible df(1)
        output, you are suggested to specify the following attributes:

            - f_bsize - preferred size of file blocks, in bytes
            - f_frsize - fundamental size of file blcoks, in bytes
                [if you have no idea, use the same as blocksize]
            - f_blocks - total number of blocks in the filesystem
            - f_bfree - number of free blocks
            - f_files - total number of file inodes
            - f_ffree - nunber of free file inodes
        """
        print "statfs"
        return os.statvfs(".")

    def fsinit(self):
        print "Starting Lindfs"

    class XmpFile(object):

        def __init__(self, path, flags, *mode):
            print "File: init", path
            self.file = os.fdopen(os.open("." + path, flags, *mode),
                                  flag2mode(flags))
            self.fd = self.file.fileno()

        def read(self, length, offset):
            print "File: read"
            self.file.seek(offset)
            return self.file.read(length)

        def write(self, buf, offset):
            print "File: write"
            self.file.seek(offset)
            self.file.write(buf)
            return len(buf)

        def release(self, flags):
            print "File: release"
            self.file.close()

        def _fflush(self):
            print "File: flush"
            if 'w' in self.file.mode or 'a' in self.file.mode:
                self.file.flush()

        def fsync(self, isfsyncfile):
            print "File: fsync"
            self._fflush()
            if isfsyncfile and hasattr(os, 'fdatasync'):
                os.fdatasync(self.fd)
            else:
                os.fsync(self.fd)

        def flush(self):
            print "File: flush"
            self._fflush()
            # cf. xmp_flush() in fusexmp_fh.c
            os.close(os.dup(self.fd))

        def fgetattr(self):
            print "File: fgetattr"
            return os.fstat(self.fd)

        def ftruncate(self, len):
            print "File: ftruncate"
            self.file.truncate(len)

        def lock(self, cmd, owner, **kw):
            print "File: lock"
            # The code here is much rather just a demonstration of the locking
            # API than something which actually was seen to be useful.

            # Advisory file locking is pretty messy in Unix, and the Python
            # interface to this doesn't make it better.
            # We can't do fcntl(2)/F_GETLK from Python in a platfrom independent
            # way. The following implementation *might* work under Linux. 
            #
            # if cmd == fcntl.F_GETLK:
            #     import struct
            # 
            #     lockdata = struct.pack('hhQQi', kw['l_type'], os.SEEK_SET,
            #                            kw['l_start'], kw['l_len'], kw['l_pid'])
            #     ld2 = fcntl.fcntl(self.fd, fcntl.F_GETLK, lockdata)
            #     flockfields = ('l_type', 'l_whence', 'l_start', 'l_len', 'l_pid')
            #     uld2 = struct.unpack('hhQQi', ld2)
            #     res = {}
            #     for i in xrange(len(uld2)):
            #          res[flockfields[i]] = uld2[i]
            #  
            #     return fuse.Flock(**res)

            # Convert fcntl-ish lock parameters to Python's weird
            # lockf(3)/flock(2) medley locking API...
            op = { fcntl.F_UNLCK : fcntl.LOCK_UN,
                   fcntl.F_RDLCK : fcntl.LOCK_SH,
                   fcntl.F_WRLCK : fcntl.LOCK_EX }[kw['l_type']]
            if cmd == fcntl.F_GETLK:
                return -EOPNOTSUPP
            elif cmd == fcntl.F_SETLK:
                if op != fcntl.LOCK_UN:
                    op |= fcntl.LOCK_NB
            elif cmd == fcntl.F_SETLKW:
                pass
            else:
                return -EINVAL

            fcntl.lockf(self.fd, op, kw['l_start'], kw['l_len'])


    def main(self, *a, **kw):

        self.file_class = self.XmpFile

        return Fuse.main(self, *a, **kw)


def main():

    usage = """
Userspace nullfs-alike: mirror the filesystem tree from some point on.

""" + Fuse.fusage

    lind.load_fs()


    server = Xmp(version="%prog " + fuse.__version__,
                 usage=usage,
                 dash_s_do='setsingle')

    # Disable multithreading: if you want to use it, protect all method of
    # XmlFile class with locks, in order to prevent race conditions
    server.multithreaded = False

    server.parser.add_option(mountopt="root", metavar="PATH", default='/',
                             help="mirror filesystem from under PATH [default: %default]")
    server.parse(values=server, errex=1)

    

    server.main()
    print "Persisting Metadata"
    lind.persist_metadata("lind.metadata")


if __name__ == '__main__':
    main()
