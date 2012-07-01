#!/bin/bash
# Chris Matthews <cmatthew@cs.uvic.ca>
# Try to ease the burden of building lind (repy, nacl, and toolchain)


# Uncomment this to print each command as they are executed
# ≈set -o xtrace

# Uncomment this for debugging. Will stop B on any failed commands 
# set -o errexit

# Uncomment this to dump time profiling information out to a file to see where the script is slow
# PS4='+ $(date "+%s.%N")\011 '
# exec 3>&2 2> bashstart.$$.log
# set -x
trap 'echo "All done."' EXIT
mode=opt-linux


# 
#
#call this instead of echo, then we can do things like log and print to notifier
function print {
    echo $1
    #notify-send --icon=/usr/share/icons/gnome/256x256/apps/utilities-terminal.png "Build Script" "$1" >& /dev/null
}


#
# wipe the entire modular build toolchain build tree, then rebuild it
# Warning: this can take a while!
function clean_toolchain {
     cd ~/lind/native_client/
     rm -rf out
}


# install many of the packages this project needs.
# Uses apt-get to setup the system.
# 
function install_deps {
    set -o errexit
    sudo aptitude install build-essential git-core subversion python2.6 python-dev python2.6-dev texinfo texlive gcc-multilib g++-multilib libsdl1.2-dev texinfo libcrypto++-dev libssl-dev lib32ncurses5-dev m4
 
    cd ~
    svn checkout http://src.chromium.org/svn/trunk/tools/depot_tools
    echo '\nexport PATH=$PATH:~/depot_tools' >> ~/.bashrc
 
}


# Compile liblind and the compoent programs.
# 
# 
function build_liblind {
    echo -ne "Building liblind... "
    cd ~/lind/misc/liblind
    make clean all > /dev/null
    echo "done."

}

cp="rsync -ra"


# Copy the toolchain files into the repy subdir.
#
# 
function install_to_path {
    # nothing should fail here.
    set -o errexit

    #make sure repy path is set
    if [ -z "$REPY_PATH" ]; then
       echo "Need to set REPY_PATH"
       exit 1
    fi 

    echo "Injecting Libs into RePy install"
    cd ~/lind/
    nacl_base=/home/lind/lind/native_client

    print "Sendning NaCl stuff to $REPY_PATH"
    base="${REPY_PATH}/lib"
    echo "Base is $base"
    mkdir -p $base
    bin=$REPY_PATH/bin
    mkdir -p $bin
    mkdir -p $base/glibc
    mkdir -p $base/libs
    mkdir -p $base/include

    $cp ${nacl_base}/scons-out/${mode}-x86-64/staging/ $bin/

    #install script
    cp -f lind.sh $bin/lind
    chmod +x $bin/lind

    $cp $nacl_base/out/install/glibc_64/nacl64/lib/ $base/glibc/
    $cp $nacl_base/out/install/glibc_64/nacl64/include/ $base/include
    $cp $nacl_base/out/install/full-gcc-glibc/nacl64/lib64/  $base/libs/
    $cp $nacl_base/out/install/glibc_64/nacl64/lib/  $base/libs/
    $cp $nacl_base/out/install/nacl_libs_glibc_64/nacl64/lib/  $base/libs/

}


# Build then copy the SDK specific parts of the toolchain.
#
# 
function build_sdk {
    build_glibc_gcc
    echo "Copying SDK"
    base="${REPY_PATH}"
    nacl_base=/home/lind/lind/native_client
    mkdir -p $base/sdk
    $cp $nacl_base/toolchain/linux_x86 $base/sdk/
    $cp /home/lind/lind/sdk_examples/ $base/sdk/examples
}


# Run the RePy unit tests.
#
#
function test_repy {
    cd $REPY_PATH/repy/
    set +o errexit  # some of our unit tests fail
    for file in ut_lind_*; do 
	    echo $file 
        #trap 'python2.6 ${REPY_PATH}/repy/repy.py --safebinary ${REPY_PATH}/repy/restrictions.lind ${REPY_PATH}/repy/lind_server.py $@' INT TERM EXIT
        trap ';' TERM
        python $file
	    #trap 'python $file' INT TERM EXIT  
    done

    # run the struct test
    file=ut_seattlelibtests_teststruct.py
    echo $file 
    python $file  

}


# Run the applications test stuites.
#
#
function test_apps {
    set +o errexit
    cd ~/lind/misc/tests
	./test.sh
}

# Check the REPY_PATH location to make sure it is safe to be installing stuff there.
#
#
function check_install_dir {
    #make sure repy path is set
    if [ -z "$REPY_PATH" ]; then
       echo "Need to set REPY_PATH"
       exit 1
    fi 
    
    # and if it does not exit, make it.
    if [ ! -d "$REPY_PATH" ]; then
	mkdir -p $REPY_PATH
    fi

}


# Trigger the build of the glibc toolchain.
#
#
function build_toolchain {
    cd ~/lind/native_client
    # the whole build always fails in the nacl tests
    set +o errexit 
    python tools/modular-build/build.py -b
    set -o errexit
}


# Install repy into $REPY_PATH with the prepare_tests script.
#
#
function build_repy {
    check_install_dir

    #make sure repy path is set
    if [ -z "$REPY_PATH" ]; then
       echo "Need to set REPY_PATH"
       exit 1
    fi 

    set -o errexit
    here=`pwd`
    
    repy_loc=${REPY_PATH}/repy/

    # remove any old copies
    rm -rf $repy_loc
    mkdir -p $repy_loc
    repy_src=~/repyv2nacl/nacl_repy/
    print "Building Repy in $repy_src to $REPY_PATH" 
    cd $repy_src
    python preparetest.py -t $repy_loc
    cp ${repy_loc}serialize.repy ${repy_loc}serialize.py
    print "Done building Repy in $repy_loc"
    cd seattlelib
    set -o errexit
    for file in *.mix
    do
	~/lind/misc/check_inlcudes.sh $file
	
    done
    set +o errexit
    etags  --language-force=python *.mix *.repy
    cd $here
}


# Update, build and test everything. If there is a problem, freak out.
#
#
function nightly_build {
    set -o errexit
    # Clean
    # clean_install
    # clean_nacl
    # clean_toolchain
    # check_install_dir
    # Update
    ~/lind/misc/global_update.sh

    # build
    # build_toolchain
    # build_rpc
    # build_glibc
    # build_nacl
    # build_repy
    # build_sdk
	# install_to_path

    # test repy
    test_repy

    # test glibc
    test_glibc

    # test applications
    test_apps

}

function clean_install {
    rm -rf $REPY_PATH
    touch $REPY_PATH
}


# Run the NaCl build.
#
#
function build_nacl {
     print "Building NaCl"
     cd ~/lind/native_client/
     # first build standard NaCl
     ./scons --verbose --mode=${mode},nacl platform=x86-64 -j16 -k
     # and check
     rc=$?
     if [ "$rc" -ne "0" ]; then
	     print "NaCl Build Failed($rc)"
	     echo -e "\a"
	     exit $rc
     fi

     # and now the glibc version
     ./scons --verbose --mode=${mode},nacl platform=x86-64 --nacl_glibc -j16 -k
     # and check
     rc=$?
     if [ "$rc" -ne "0" ]; then
	     print "NaCl Build Failed($rc)"
	     echo -e "\a"
	     exit $rc
     fi

     print "Done building NaCl $rc"
     install_to_path
}


# Run clean on nacl build.
#
#
function clean_nacl {
     cd ~/lind/native_client/
     ./scons --mode=${mode},nacl platform=x86-64 --nacl_glibc -c
     print "Done Cleaning NaCl"
}


# Build glibc from source
#
#
function build_glibc {
     # the build is long and borning, so execute this first if it exists
     type -P fortune &>/dev/null && fortune || echo "Fortune Not Found. Skipping." 

     echo -ne "Copy component.h header to glibc: "
     cd ~/lind/misc/liblind
     cp -f component.h ~/lind/nacl-glibc/sysdeps/nacl/
     echo "done."

     echo "Building glibc"

     # if extra files (like editor temp files) are in the subdir glibc tries to compile them too.
     # move them here so they dont cause a problem
     cd ~/lind/nacl-glibc/sysdeps/nacl/
     shopt -s nullglob
     for f in .#*;
     do
	 print "moving editor backupfile ${f} so it does not get caught in build."
	 mv -f ${f} .
     done

     #turns out this works better if you do it from the nacl base dir
     cd ~/lind/native_client
     python tools/modular-build/build.py glibc-src -s --allow-overwrite -b
     # python tools/modular-build/build.py
     #../sysdeps/nacl/nacl_stat.h:102: warning: its scope is only this definition or declaration, which is probably not what you want
     python tools/modular-build/build.py -s -b glibc_64 2>&1 | tee build.stderr.log | grep -vE "warning: ignoring old commands for target|warning: overriding commands for target| warning: \‘struct stat*\’ declared inside parameter list|../sysdeps/nacl/nacl_stat.h:102:" | grep -e '^../sysdeps/nacl/' -e '^../socket/' -e '^../misc/' -e '^../inet/' -e 'res_send.c' -e 'poll.c' | grep -e 'warning' -e 'error'
     rc=${PIPESTATUS[0]}
     sync
     if [ "$rc" -ne "0" ]; then
		 cat build.stderr.log |  grep -vE "warning: ignoring old commands for target|warning: overriding commands for target" | tail -n 200
		 print "Glibc Build failed"
		 echo -e "\a"
		 exit $rc
     else
		 print "Building glibc_64 Succeeded."
     fi
	 cd ~/lind/nacl-glibc/sysdeps/nacl/
	 etags *.c
     print "Done partial build."
} 


# Build the GCC toolchain
#
#
function build_glibc_gcc {
    cd ~/lind/native_client

    best_target="full-gcc-glibc"
    python tools/modular-build/build.py $best_target -b > build.good.stdout.log 2> build.good.stderr.log
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
    install_to_path

    print "Done Build"
}


# Run the glibc tester
#
#
function glibc_tester {
    set -o errexit

    cd ~/lind/misc/glibc_test/
    make clean all
    cd ..
	rm -rfv lind.metadata linddata.*
    lind ~/lind/misc/glibc_test/glibc_tester.nexe
}


# Run the RPC generator
#
#
function build_rpc {
    set -o errexit
    cd ~/lind/misc/rpcgen/
    python syscall_gen.py | indent 
    type -P indent &>/dev/null && indent lind_rpc_gen.c -o lind_rpc_gen.c || echo "Indent Not Found. Skipping reformatting rpc code." 
    mv -v lind_rpc_gen.* ~/lind/nacl-glibc/sysdeps/nacl/
}

PS3="build what: " 
list="all repy nacl glibc cleantoolchain cleannacl install install_deps liblind test_repy test_glibc test_apps sdk rpc test nightly"
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

# all scripts assume we start here
cd /home/lind/lind/ 
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
    elif [ "$word" = "sdk" ]; then
	    build_sdk
    elif [ "$word" = "all" ]; then
        build_rpc
	    build_glibc
	    build_nacl
	    build_repy
	    install_to_path
        test_glibc
    elif [ "$word" = "cleantoolchain" ]; then
	    print "Cleaning Toolchain"
	    clean_toolchain
    elif [ "$word" = "install" ]; then
	    print "Installing libs into install dir"
	    install_to_path
    elif [ "$word" = "cleannacl" ]; then
	    print "Cleaning NaCl"
	    clean_nacl
    elif [ "$word" = "liblind" ]; then
	    print "Building LibLind"
	    build_liblind
    elif [ "$word" = "test_repy" ]; then
	    print "Testing Repy"
	    test_repy
    elif [ "$word" = "test_glibc" ]; then
	    print "Testing GLibC"
	    glibc_tester
    elif [ "$word" = "test_apps" ]; then
	    print "Testing Applications"
	    test_apps
    elif [ "$word" = "test" ]; then
	    print "Testing All"
	    test_repy
	    glibc_tester
	    test_apps
    elif [ "$word" = "rpc" ]; then
	    print "Building new RPC stubs"
	    build_rpc
    elif [ "$word" = "nightly" ]; then
	    print "Nightly Build"
	    nightly_build
    elif [ "$word" = "install_deps" ]; then
	    print "Installing Dependicies"
	    install_deps
    else 
	    echo "Error: Did not find a build target named $word. Exiting..."
	    exit 1
    fi
done


END_TIME=$(date +%s)
DIFF=$(( $END_TIME - $START_TIME ))
echo "It took $DIFF seconds"
echo -e "\a"


