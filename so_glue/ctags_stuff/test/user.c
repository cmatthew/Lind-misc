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
	int num_of_tests_t = 8;
	
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
	xx = 400l;
	yy = 500l;
	printf(">%ld<, %ld\n", foo_long(xx,yy,x), xx-yy);
	assert(foo_long(xx, yy, x)==(long)(-100));
	num_of_tests++; 
	
	printf("printing float %f\n", foo_float((float) 2.33, y));
	assert(foo_float( (float)2.33, y)==(float)2.33);
	printf("passed float(float, int)\n");
	num_of_tests++;
	printf("double: %Lf\n",foo_double(x, (double)23.3F, (double)23.1223F));




	printf("Passed %d / %d\n", num_of_tests, num_of_tests_t);
//	printf("hmmm: %d\n", MD4_Init(test_MD4));
	return 0;
}

