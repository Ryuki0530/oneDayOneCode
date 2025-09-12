#include <stdio.h>

#define MAX_SIZE 100

int main(void) {
    int arr[MAX_SIZE];
    int n = 0, x;

    // 入力
    while (n < MAX_SIZE) {
        if (scanf("%d", &x) != 1) break;
        if (x == 0) break;
        arr[n++] = x;
    }

    if (n == 0) {
        // 入力が0のみの場合
        printf("0\n0\n");
        return 0;
    }

    int max = arr[0], min = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max) max = arr[i];
        if (arr[i] < min) min = arr[i];
    }

    printf("%d\n%d\n", max, min);
    return 0;
}