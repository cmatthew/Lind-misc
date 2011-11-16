#!/bin/bash
# Chris Matthews <cmatthew@cs.uvic.ca>
# Try to ease the burden of building lind (repy, nacl, and toolchain)
#set -o xtrace
#set -o errexit

#call this instead of echo, then we can do things like log and print to notifier
function print {
    echo $1
    #notify-send --icon=/usr/share/icons/gnome/256x256/apps/utilities-terminal.png "Build Script" "$1" >& /dev/null
}

#wipe the entire modular build toolchain build tree
function clean_toolchain {
     cd ~/lind/native_client/
     rm -rf out
     python tools/modular-build/build.py -b
}

# install many of the packages this project needs
function install_deps {
    set -o errexit
    sudo aptitude install build-essential git-core subversion python2.6 python-dev python2.6-dev texinfo texlive gcc-multilib g++-multilib libsdl1.2-dev texinfo libcrypto++-dev libssl-dev lib32ncurses5-dev m4
 
    cd ~
    svn checkout http://src.chromium.org/svn/trunk/tools/depot_tools
    echo '\nexport PATH=$PATH:~/depot_tools' >> ~/.bashrc
 
}


# copy the toolchain files into the repy subdir
function inject_libs_into_repy {
    set -o errexit
    nacl_base=./native_client
    print "Sendning NaCl stuff to $REPY_PATH"
    base="${REPY_PATH}lind"
    echo "Base is $base"
    mkdir -p $base
    bin=$base/bin
    mkdir -p $bin
    mkdir -p $base/glibc
    mkdir -p $base/libs
    mkdir -p $base/include
    pwd
    cp -rf ${nacl_base}/scons-out/dbg-linux-x86-64/staging/* $bin/
    cp -f lind.sh $bin/lind
    chmod +x $bin/lind
    cp -rf $nacl_base/out/install/glibc_64/nacl64/lib/* $base/glibc/
    cp -rf $nacl_base/out/install/glibc_64/nacl64/include/ $base/include
    cp -rf $nacl_base/out/install/full-gcc-glibc/nacl64/lib64/*  $base/libs/
    cp -rf $nacl_base/out/install/glibc_64/nacl64/lib/*  $base/libs/
    cp -rf $nacl_base/out/install/nacl_libs_glibc_64/nacl64/lib/*  $base/libs/
    build_sdk
}

# copy the SDK specific parts of the toolchain
function build_sdk {
    nacl_base=./native_client
    mkdir -p $base/sdk
    cp -rf $nacl_base/out/install/full-gcc-glibc/* $base/sdk/
    cp -rf ./sdk_examples $base/sdk/examples
}


# install repy into $REPY_PATH with the prepare_tests script
function build_repy {
    if [ -z "$REPY_PATH" ]; then
       echo "Need to set REPY_PATH"
       exit 1
    fi 
    here=`pwd`
    rm -rf $REPY_PATH
    mkdir -p $REPY_PATH
    repy_src=~/repyv2nacl/nacl_repy/
    print "Building Repy in $repy_src to $REPY_PATH" 
    cd $repy_src
    python preparetest.py -t $REPY_PATH
    print "Done building Repy in $REPY_PATH"
    cd $here
    inject_libs_into_repy
}

# How NaCl compiles a program for 
# /home/lind/lind/native_client/toolchain/linux_x86/bin/nacl64-gcc 
# -o scons-out/nacl-x86-64-glibc/obj/tests/lind/glibc_test.o 
# -c 
# -std=gnu99 
# -Wall 
# -Wextra 
# -O2 
# -fomit-frame-pointer 
# -Wall 
# -fdiagnostics-show-option 
# -pedantic 
# -Werror 
# -m64 
# -D_GNU_SOURCE=1 
# -D__STDC_FORMAT_MACROS=1 
# -D_BSD_SOURCE=1 
# -D_POSIX_C_SOURCE=199506 
# -D_XOPEN_SOURCE=600 
# -I/home/lind/lind 
# -I/home/lind/tmp/repy/include 
# tests/lind/glibc_test.c


# Run the NaCl build
function build_nacl {
     print "Building NaCl"
     cd ~/lind/native_client/
     ./scons --verbose --mode=dbg-linux,nacl platform=x86-64 --nacl_glibc -j16 
     rc=$?
     if [ "$rc" -ne "0" ]; then
	 print "NaCl Build Failed($rc)"
	 echo -e "\a"
	 exit $rc
     fi
     print "Done building NaCl $rc"
     inject_libs_into_repy

}

# Run clean on nacl build
function clean_nacl {
     cd ~/lind/native_client/
     ./scons --mode=dbg-linux,nacl platform=x86-64 --nacl_glibc -c
     print "Done Cleaning NaCl"
}


#
function build_glibc {
     fortune
     print "Building glibc"
     # if extra files (like editor temp files) are in the subdir glibc tries to compile them too.
     # move them here so they dont cause a problem
     mv -f nacl-glibc/sysdeps/nacl/.#* .
     #turns out this works better if you do it from the nacl base dir
     cd ~/lind/native_client
     python tools/modular-build/build.py glibc-src -s --allow-overwrite -b
     # python tools/modular-build/build.py
     python tools/modular-build/build.py glibc_64 -b -s > build.stdout.log 2> build.stderr.log
     sync
     rc=$?
     if [ "$rc" -ne "0" ]; then
	 cat build.stderr.log |  grep -vE "warning: ignoring old commands for target|warning: overriding commands for target" | tail -n 200
	 print "Glibc Build failed"
	 echo -e "\a"
	 exit $rc
     else
	 print "Building glibc_64 Succeeded."
     fi
     print "Done partial build."
     python tools/modular-build/build.py nacl_libs_glibc_64 -b > build.good.stdout.log 2> build.good.stderr.log
     sync
     rc=$?
     if [ "$rc" -ne "0" ]; then
	 print "Build failed"
	 echo -e "\a"
	 exit $rc
     else
	 print "Full build Succeeded."
     fi
     python tools/modular-build/build.py
     print "Done building GLibC"
     inject_libs_into_repy
     print "Done Build"
}

function build_glibc_fast {
     print "Building glibc FAST"
     #turns out this works better if you do it from the nacl base dir
     cd ~/lind/native_client
     python tools/modular-build/build.py glibc-src -s --allow-overwrite -b
     python tools/modular-build/build.py
     python tools/modular-build/build.py hello_glibc -b > build.log
     rc=$?
     if [ "$rc" -ne "0" ]; then
     	 print "Build failed"
	 echo -e "\a"
     	 exit $rc
     fi
     python tools/modular-build/build.py
     print "Done building GLibC fast."
}


function inplace {
    rsync -rlEtv  ./nacl-glibc ~/lind/native_client/out/build/glibc_64
    cd ~/lind/native_client/out/build/glibc_64
    make
    cd ../../..
}


function run_lind {
    print "Running lind."
    python2.6 $REPY_PATH/repy.py --safebinary $REPY_PATH/restrictions.lind $REPY_PATH/lind_server.py /home/lind/lind/native_client/scons-out/nacl-x86-64-glibc/staging/glibc_test.nexe
    print "Done Running."
}


function watch {
    print "Watch for what?"
    read what
    grep $what ~/lind/native_client/tools/modular-build/build.log    
}

PS3="build what: " 
list="repy nacl glibc run cleantoolchain cleannacl glibcfast inplace install install_deps"
word=""
if  test -z "$1" 
then
    select foo in $list;
    do
       args=("$foo")
       break
    done
else
    args=("$@")
fi

START_TIME=$(date +%s)

echo ${args[0]} ${args[1]}
ELEMENTS=${#args[@]}
for  (( i=0;i<$ELEMENTS;i++));
do
    word=${args[${i}]}
    if [ "$word" = "repy" ]; then
	build_repy
    elif [ "$word" = "nacl" ]; then
	build_nacl
    elif [ "$word" = "glibc" ]; then
	build_glibc
    elif [ "$word" = "glibcfast" ]; then
	build_glibc_fast
    elif [ "$word" = "all" ]; then
	build_glibc
	build_nacl
	build_repy
	run_lind
    elif [ "$word" = "run" ]; then
	run_lind
    elif [ "$word" = "cleantoolchain" ]; then
	print "Cleaning Toolchain"
	clean_toolchain
    elif [ "$word" = "inplace" ]; then
	print "Inplace Build"
	inplace
    elif [ "$word" = "install" ]; then
	print "Installing into Repy"
	prepare_nacl_repy
    elif [ "$word" = "cleannacl" ]; then
	print "Cleaning NaCl"
	clean_nacl
    elif [ "$word" = "install_deps" ]; then
	print "Installing Dependicies"
	install_deps
    fi
done


END_TIME=$(date +%s)
DIFF=$(( $END_TIME - $START_TIME ))
echo "It took $DIFF seconds"
echo -e "\a"


