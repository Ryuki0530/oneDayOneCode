#include <stdio.h>

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) {
        return 1;
    }

    if (N <= 0) {
        printf("None\n");
        return 0;
    }

    int prev, current;
    if (scanf("%d", &prev) != 1) {
        return 1;
    }

    int count = 1;
    int found = 0;

    for (int i = 1; i < N; i++) {
        if (scanf("%d", &current) != 1) {
            return 1;
        }

        if (current == prev) {
            count++;
        } else {
            if (count >= 3) {
                printf("%d %d\n", prev, count);
                found = 1;
            }
            prev = current;
            count = 1;
        }
    }

    if (count >= 3) {
        printf("%d %d\n", prev, count);
        found = 1;
    }

    if (!found) {
        printf("None\n");
    }

    return 0;
}