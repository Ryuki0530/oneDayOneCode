#include <stdio.h>

unsigned int count_bits(unsigned int value)
{
	unsigned int count = 0;

	while (value != 0u) {
		count += value & 1u;
		value >>= 1;
	}

	return count;
}

int main(void)
{
	const unsigned int values[] = {0u, 1u, 7u, 10u, 255u};
	const unsigned int value_count = sizeof(values) / sizeof(values[0]);

	for (unsigned int i = 0; i < value_count; ++i) {
		printf("%u: %u\n", values[i], count_bits(values[i]));
	}

	return 0;
}
