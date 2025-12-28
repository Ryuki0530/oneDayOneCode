#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int W;
    if (scanf("%d", &W) != 1 || W <= 0) {
        return 0;
    }

    long long *buf = (long long *)malloc(sizeof(long long) * W);
    if (!buf) {
        return 0;
    }

    long long sum = 0;
    int idx = 0;
    int count = 0;
    long long x;

    while (scanf("%lld", &x) == 1) {
        if (count == W) {
            sum -= buf[idx];
        } else {
            count++;
        }

        buf[idx] = x;
        sum += x;

        if (count == W) {
            double avg = (double)sum / (double)W;
            double diff = (double)x - avg;
            printf("%.3f\n", diff);
        }

        idx++;
        if (idx == W) {
            idx = 0;
        }
    }

    free(buf);
    return 0;
}