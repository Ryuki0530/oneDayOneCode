#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int W, N;
    if (scanf("%d", &W) != 1) return 0;
    if (scanf("%d", &N) != 1) return 0;

    long long *buf = (long long *)malloc(sizeof(long long) * W);
    if (!buf) return 0;

    int idx = 0, count = 0;
    long long sum = 0;

    for (int i = 0; i < N; ++i) {
        long long x;
        if (scanf("%lld", &x) != 1) break;

        if (count == W) {
            sum -= buf[idx];
        } else {
            ++count;
        }

        buf[idx] = x;
        sum += x;
        idx = (idx + 1) % W;

        printf("%lld\n", sum);
    }

    free(buf);
    return 0;
}