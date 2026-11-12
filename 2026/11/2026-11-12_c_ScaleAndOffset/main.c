	#include <assert.h>
	#include <inttypes.h>
	#include <stdint.h>
	#include <stdio.h>

	uint32_t scaleAndOffset(uint32_t x)
	{
		__asm__(
			"shll $3, %[value]\n\t"
			"addl $13, %[value]"
			: [value] "+r" (x)
			:
			: "cc"
		);

		return x;
	}

	int main(void)
	{
		const uint32_t values[] = {
			0u,
			1u,
			10u,
			536870912u,
			UINT32_C(4294967295)
		};

		for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
			uint32_t x = values[i];
			uint32_t actual = scaleAndOffset(x);
			uint32_t expected = x * 8u + 13u;

			assert(actual == expected);
			printf("%" PRIu32 " -> %" PRIu32 "\n", x, actual);
		}

		return 0;
	}
