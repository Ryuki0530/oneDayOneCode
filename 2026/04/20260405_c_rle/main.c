#include <stdio.h>

int main(void) {
	int N;
	if (scanf("%d", &N) != 1) {
		return 0;
	}

	int prev, x;
	int count = 1;

	if (scanf("%d", &prev) != 1) {
		return 0;
	}

	for (int i = 1; i < N; i++) {
		if (scanf("%d", &x) != 1) {
			return 0;
		}

		if (x == prev) {
			count++;
		} else {
			printf("%d %d\n", prev, count);
			prev = x;
			count = 1;
		}
	}

	printf("%d %d\n", prev, count);
	return 0;
}
