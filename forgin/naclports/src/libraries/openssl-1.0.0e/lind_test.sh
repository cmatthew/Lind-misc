#! /bin/bash

testpath=../../out/repository-i686/openssl-1.0.0e/test/

tests="asn1test bctest bftest bntest casttest destest dhtest dsatest dummytest ecdhtest ecdsatest ectest enginetest evp_test exptest hmactest ideatest igetest jpaketest md2test md4test md5test mdc2test randtest rc2test re4test rc5test rmdtest rsa_test sha1test sha256t sha512t shatest ssltest test_padlock wp_test"

for test in $tests; do

    echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
    echo $test
    echo ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
    lind $testpath$test

done