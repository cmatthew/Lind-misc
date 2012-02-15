#include "tmptest.h"
#include <stdio.h>
#include <assert.h>
int main () {
	int x, y;
	x = 100;
	y = 200; 
	printf("\nfoo passing in %d, getting out %d\n\n", x, foo(x));
	assert(x == foo(x));
	printf("\nfoo add passing in %d, %d and getting out %d\n\n", x,y, foo_add(x, y));
	assert(foo_add(x,y) == 300);

//	printf("hmmm: %d\n", MD4_Init(test_MD4));
	return 0;
}

