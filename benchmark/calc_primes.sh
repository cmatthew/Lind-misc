#! /bin/bash

out=output.lind.$$
real=output.real.$$
nacl=output.nacl.$$
rm -rf $out $real $nacl

for((i = 0; i < 100; i++))
do
    echo "NaCl $i"	
    sel_ldr ~/lind/native_client/scons-out/nacl-x86-64/obj/tests/lind/glibc_test.nexe >> $nacl
	echo "Lind $i"	
	lind ./pi.nexe >> $out # | grep ">>" # | cut -f2,3 -d \  >> $out
    killall python2.6
	echo "Native $i"	
	./pi >> $real # | grep ">>" # | cut -f2,3 -d \  >> $real

    
done
	