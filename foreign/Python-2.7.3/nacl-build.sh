#! /bin/bash

pydir=/home/lind/lind/misc/foreign/Python-2.7.3
libdir=/home/lind/tmp/lind/lib/libs
install_loc=${pydir}/nacl_build

host-gen()
{
    ./configure 

    make python Parser/pgen

    cp python hostpython
    cp Parser/pgen Parser/hostpgen

    #make distclean
}


apply-patch()
{
    patch -p1 < Python-2.7.2-xcompile.patch
}


nacl-config()
{
    CC=nacl64-gcc
    CXX=nacl64-g++
    AR=nacl64-ar
    RANLIB=nacl64-ranlib

    ./configure --host=nacl64 --prefix=${pydir}/${install_loc}
}

nacl-build()
{
    CC=nacl64-gcc
    CXX=nacl64-g++
    AR=nacl64-ar
    RANLIB=nacl64-ranlib

    #./configure --host=nacl64 --prefix=${pydir}/${install_loc}

    make HOSTPYTHON=./hostpython HOSTPGEN=./Parser/hostpgen BLDSHARED="nacl64-gcc -shared" CROSS_COMPILE=nacl64- CROSS_COMPILE_TARGET=yes HOSTARCH=nacl64 BUILDARCH=x86_64-linux-gnu
}


nacl-install()
{
    CC=nacl64-gcc
    CXX=nacl64-g++
    AR=nacl64-ar
    RANLIB=nacl64-ranlib
    make install HOSTPYTHON=./hostpython HOSTPGEN=./Parser/hostpgen BLDSHARED="nacl64-gcc -shared" CROSS_COMPILE=nacl64- CROSS_COMPILE_TARGET=yes HOSTARCH=nacl64 BUILDARCH=x86_64-linux-gnu
}


naclpy-install()
{
    mv ${pydir}/python ${install_loc}/bin/python2.7
}


all()
{
    host-gen
    clean
    apply-patch
    nacl-config
    nacl-build
    nacl-install
    lib-install
}


all-nopatch()
{
    host-gen
    clean
    nacl-config
    nacl-build
    nacl-install
    lib-install
}


lib-install()
{
    echo "lib-install not implemented"
    #cp $pydir/libpython2.7.so.1.0 $libdir
}


lib-uninstall()
{
    echo "lib-uninstall not implemented"
    #rm -f $libdir/libpython2.7.so.1.0
}

clean()
{
    make clean
}

if [ -z $1 ]; then
    cmd="all-np"
else
    cmd=$1
fi

case "$1" in

    "setup" )
	host-gen
	;;

    "patch" )
	apply-patch
	;;

    "config" )
	nacl-config
	;;

    "build" )
	nacl-build
	;;

    "naclpy-install" )
	naclpy-install
	;;

    "qb" )
	nacl-build
	naclpy-install
	;;

    "install" )
	nacl-install
	lib-install
	;;

    "nacl-install" )
	nacl-install
	;;

    "lib-install" )
	lib-install
	;;

    "clean" )
	clean
	;;

    "all" )
	all
	;;

    "all-np" )
	all-nopatch
	;;

    * )
	echo "invalid option ${1}"
	;;

esac