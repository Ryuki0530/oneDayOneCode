#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void *my_memmove(void *dest, const void *src, size_t n)
{
	unsigned char *d = (unsigned char *)dest;
	const unsigned char *s = (const unsigned char *)src;
	uintptr_t dest_addr = (uintptr_t)d;
	uintptr_t src_addr = (uintptr_t)s;

	if (n == 0 || d == s) {
		return dest;
	}

	if (dest_addr > src_addr && dest_addr - src_addr < n) {
		for (size_t i = n; i > 0; --i) {
			d[i - 1] = s[i - 1];
		}
	} else {
		for (size_t i = 0; i < n; ++i) {
			d[i] = s[i];
		}
	}

	return dest;
}

static int check(const char *name, const char *actual, const char *expected)
{
	int ok = 1;
	for (size_t i = 0; actual[i] != '\0' || expected[i] != '\0'; ++i) {
		if (actual[i] != expected[i]) {
			ok = 0;
			break;
		}
	}
	printf("%s: %s\n", name, ok ? "PASS" : "FAIL");
	return ok;
}

int main(void)
{
	int passed = 0;

	char forward[] = "ABCDE";
	my_memmove(forward + 1, forward, 4);
	passed += check("overlap forward", forward, "AABCD");

	char backward[] = "ABCDE";
	my_memmove(backward, backward + 1, 4);
	passed += check("overlap backward", backward, "BCDEE");

	char separate[6] = "ABCDE";
	my_memmove(separate, "XYZ", 3);
	passed += check("non-overlap", separate, "XYZDE");

	char unchanged[] = "ABCDE";
	my_memmove(unchanged, unchanged, 0);
	passed += check("zero bytes", unchanged, "ABCDE");

	printf("%d/4 tests passed\n", passed);
	return passed == 4 ? 0 : 1;
}
