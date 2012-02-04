#include "tmptest.h"
#include <stdio.h>

int main () {
	int x;
	x = 128;
	printf("foo passing in %d, getting out %d\n", x, foo(x));
	return 0;
}

