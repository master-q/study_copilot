#include <stdio.h>
#include <stdint.h>
#include "copilot.h"

void fib_out(uint64_t var)
{
	printf("(%llu)\n", var);
}

int main()
{
	int i;

	for (i = 0; i < 50; i++) {
		step();
	}
	return 0;
}
