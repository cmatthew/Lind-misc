#include "tmptest.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
int main () {
	int x, y;
	x = 300;
	y = 300; 
	printf("\nfoo passing in %d, getting out %d\n\n", x, foo(x));
	printf("\nfoo_add passing in %d, %d and getting out %d\n\n", x,y, foo_add(x, y));
	printf("\none_line: %s,\n %s\n^^3 lines\n", "hello\nworld", one_line("hello\nworld"));

	assert(x == foo(x));
	printf("passed: int foo(int x)\n");
	assert(foo_add(x,y) == x+y);
	printf("passed: int foo_add(int x, int y)\n");
	assert(strcmp("hello world", one_line("hello\nworld"))==0);
	printf("passed: char * one_line(char *)\n");
	
//	printf("hmmm: %d\n", MD4_Init(test_MD4));
	return 0;
}

