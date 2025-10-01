#include <stdio.h>
#include <limits.h>

int main(void) {
    int N, a;
    int max = INT_MIN;
    int min = INT_MAX;

    scanf("%d", &N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &a);
        if (a > max) max = a;
        if (a < min) min = a;
    }
    printf("%d\n", max - min);
    return 0;
}