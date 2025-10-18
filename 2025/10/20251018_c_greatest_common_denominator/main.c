#include <stdio.h>

int gcd(int a, int b) {
    while (b != 0) {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

int main(void) {
    int n;
    if (scanf("%d", &n) != 1 || n < 2) {
        return 0;
    }

    int prev;
    if (scanf("%d", &prev) != 1) {
        return 0;
    }

    for (int i = 1; i < n; ++i) {
        int curr;
        if (scanf("%d", &curr) != 1) {
            return 0;
        }
        printf("%d\n", gcd(prev, curr));
        prev = curr;
    }

    return 0;
}