#include <stdio.h>

int main() {
    int N, num, max;
    if (scanf("%d", &N) != 1 || N <= 0) {
        return 1;
    }
    if (scanf("%d", &max) != 1) {
        return 1;
    }
    for (int i = 1; i < N; i++) {
        if (scanf("%d", &num) != 1) {
            return 1;
        }
        if (num > max) {
            max = num;
        }
    }
    printf("%d\n", max);
    return 0;
}