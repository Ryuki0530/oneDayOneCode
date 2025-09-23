#include <stdio.h>

int main(void) {
    int N, i, max, num;
    scanf("%d", &N);

    scanf("%d", &max); // 最初の値をmaxに
    for (i = 1; i < N; i++) {
        scanf("%d", &num);
        if (num > max) {
            max = num;
        }
    }

    printf("%d\n", max);
    return 0;
}