#include "tmptest.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
int main () {
	int x, y;
	char z;
	x = 300;
	y = 300;
	z = 6;
/*	printf("\nfoo passing in %d, getting out %d\n\n", x, foo(x));
	printf("\nfoo_add passing in %d, %d and getting out %d\n\n", x,y, foo_add(x, y));
	printf("\none_line: %s,\n %s\n^^3 lines\n", "hello\nworld", one_line("hello\nworld"));
	printf("\n\n\n\nfoo_char passing in %d,%d getting out %d\n\n\n\n\n", x/2, z, foo_char(5, z));
	printf("DONEDONEDONEDONEDONEDON\n");
	printf("\nfoo passing in %d, getting out %d\n\n", x, foo(x));
*/	
	
	int num_of_tests = 0;
	int num_of_tests_t = 11;
	
	//assert(9876 = foo_empty());
	//num_of_tests++;
	//printf("passed: int foo()\n");
	
	const char * one = "hello";
	const char * two = "world";
	printf ("we are here in the user.c line 28\n");
	printf("\n>>>>%s<<<<<\n", crypt(one, two));
assert(strcmp("woglQSsVNh3SM", crypt(one, two)) == 0);
	num_of_tests++;

	assert(x == foo(x));
	num_of_tests++;
	printf("passed: int foo(int x)\n");
	
	assert(foo_add(x,y) == x+y);
	num_of_tests++;
	printf("passed: int foo_add(int x, int y)\n");
	
	assert(strcmp("hello world", one_line("hello\nworld"))==0);
	num_of_tests++;
	printf("passed: char * one_line(char *)\n");
	
	assert(x == foo(x));
	num_of_tests++;
	printf("passed: int foo(int x), %hd\n", foo_char(-7,z));
	
	assert((short)(-1) == (short)foo_char(-7,z));
	printf("passed: char foo_char(int, char)\n");
	num_of_tests++;
	
	long xx, yy;
	xx = 100l;
	yy = 500l;
	printf(">%ld<, %ld\n", foo_long(xx,yy,x), xx-yy);
	assert(foo_long(xx, yy, x)==(long)(xx-yy));
	num_of_tests++; 
	
	printf("printing float %f\n", foo_float((float) 2.33, y));
	assert(foo_float( (float)2.33, y)==(float)2.33);
	printf("passed float(float, int)\n");
	num_of_tests++;

	_Bool bool;
	bool = (_Bool)22;
	printf("%d bool. %d\n", bool, foo_bool(bool));
	assert(foo_bool(bool) == bool - (_Bool)1);
	printf("passed foo_bool\n");
	num_of_tests++;

	double dx, dy;
	dx = 2.2e+23l;
	dy = 2.18e+23;
	printf("double: %f\n",foo_double(x, (double)1/(double)3, dx));

	assert(foo_double(x, dx,dy) == dx-dy);
	printf("Passed double foo_double\n");
	num_of_tests++;

	assert(strcmp("helloworld", two_strings(one,two)) == 0);
	printf("Passed char * two_strings\n");
	num_of_tests++;
	
	MD5_CTX c;
	unsigned char digest[16];
	char *out = (char *) malloc(33);
	MD5_Update(&c, "hello", 5);
	int rc = MD5_Final(digest, &c);
	printf("return code: %d\n", rc);
	int n;
	for (n = 0; n < 16; ++n) {
		snprintf(&(out[n*2]), 16*2, "%02x", (unsigned int) digest[n]);
	}
	printf ("md5 output: %s\n", out);
	printf("Passed %d / %d\n", num_of_tests, num_of_tests_t);
	
	return 0;
}

