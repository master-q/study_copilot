#include <stdio.h>
#include <stdint.h>
#include "copilot.h"

void pout_w64(uint64_t var)
{
	printf("(%llu)\n", var);
}

int32_t l_getchar(void)
{
	static int32_t ret = 'A';
	return ret++;
}

void l_putchar(int32_t c)
{
	putchar((int) c);
}

void counter_out(uint64_t var)
{
	printf("(%llu)\n", var);
}

int main()
{
	int i;

	printf(">> Fakeboot for Book:Lambda Ka Musume\n");
	for (i = 0; i < 10; i++) {
		step();
	}
	printf("\n");

	return 0;
}
