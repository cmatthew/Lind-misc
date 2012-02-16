#include "tmptest.h"
#include <stdio.h>
#include <assert.h>
int main () {
	int x, y;
	x = 300;
	y = 300; 
	printf("\nfoo passing in %d, getting out %d\n\n", x, foo(x));
	printf("\nfoo_add passing in %d, %d and getting out %d\n\n", x,y, foo_add(x, y));
	printf("\none_line: %s,\n %s\n^^3 lines\n", "hello\nworld\0", one_line("hello\nworld\0"));

	assert(x == foo(x));
	printf("passed: int foo(int x)\n");
	assert(foo_add(x,y) == x+y);
	printf("passed: int foo_add(int x, int y)\n");

	
//	printf("hmmm: %d\n", MD4_Init(test_MD4));
	return 0;
}

