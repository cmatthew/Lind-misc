#include "tmptest.h"
#include <stdio.h>

int main () {
	int x, y;
	x = 128;
	y = -20482048; 
//	printf("foo passing in %d, getting out %d\n", x, foo(x));
	printf("foo passing in %d, %d and getting out %d\n", x,y, foo_add(x, y));
	
//	printf("hmmm: %d\n", MD4_Init(test_MD4));
	return 0;
}

