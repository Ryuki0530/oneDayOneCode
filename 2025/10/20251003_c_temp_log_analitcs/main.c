#include <stdio.h>
#include <float.h>

int main(void) {
    double x;
    int count = 0;
    double min = DBL_MAX, max = -DBL_MAX, sum = 0.0;

    while (scanf("%lf", &x) == 1) {
        if (count == 0) {
            min = max = x;
        } else {
            if (x < min) min = x;
            if (x > max) max = x;
        }
        sum += x;
        count++;
    }

    if (count == 0) {
        printf("NO DATA\n");
    } else {
        printf("count: %d\n", count);
        printf("min: %.2f\n", min);
        printf("max: %.2f\n", max);
        printf("mean: %.2f\n", sum / count);
    }

    return 0;
}