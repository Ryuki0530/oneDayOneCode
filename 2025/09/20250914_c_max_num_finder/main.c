#include <stdio.h>
#include <limits.h>

int main() {
    int num, max = INT_MIN, count = 0;
    while (scanf("%d", &num) == 1 && count < 100) {
        if (num > max) {
            max = num;
        }
        count++;
    }
    if (count > 0) {
        printf("%d\n", max);
    }
    return 0;
}