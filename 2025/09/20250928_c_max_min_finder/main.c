#include <stdio.h>

int main(void) {
    int N;
    int num;
    int max;
    int min;

    scanf("%d", &N);
    scanf("%d", &num);
    
    max = min = num;
    for (int i = 1; i < N; i++) {
        scanf("%d", &num);
        if (num > max) max = num;
        if (num < min) min = num;
    }

    printf("%d %d\n", max, min);
    return 0;
}