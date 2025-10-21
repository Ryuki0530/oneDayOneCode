#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) {
        return 1;
    }

    long long max_value = 0;
    int count = 0;

    for (int i = 0; i < n; ++i) {
        long long current;
        if (scanf("%lld", &current) != 1) {
            return 1;
        }

        if (i == 0 || current > max_value) {
            max_value = current;
            count = 1;
        } else if (current == max_value) {
            count++;
        }
    }

    printf("%lld %d\n", max_value, count);
    return 0;
}