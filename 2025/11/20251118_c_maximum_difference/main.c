#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int N;
    if (scanf("%d", &N) != 1) return 0;
    long long prev, x;
    if (scanf("%lld", &prev) != 1) return 0;

    long long maxd = 0;
    for (int i = 1; i < N; ++i) {
        if (scanf("%lld", &x) != 1) return 0;
        long long d = prev - x;
        if (d < 0) d = -d;
        if (d > maxd) maxd = d;
        prev = x;
    }

    printf("%lld\n", maxd);
    return 0;
}