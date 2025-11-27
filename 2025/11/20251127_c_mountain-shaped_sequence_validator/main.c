#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) {
        return 0;
    }
    if (N < 3) {
        puts("NO");
        return 0;
    }

    int *a = (int *)malloc(sizeof(int) * N);
    if (!a) {
        return 0;
    }
    for (int i = 0; i < N; i++) {
        if (scanf("%d", &a[i]) != 1) {
            free(a);
            return 0;
        }
    }

    int i = 1;
    while (i < N && a[i - 1] < a[i]) {
        i++;
    }
    int peak = i - 1;
    if (peak <= 0 || peak >= N - 1) {
        puts("NO");
        free(a);
        return 0;
    }

    for (int j = peak + 1; j < N; j++) {
        if (a[j - 1] <= a[j]) {
            puts("NO");
            free(a);
            return 0;
        }
    }

    puts("YES");
    free(a);
    return 0;
}