#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

int main() {
    int n;
    scanf("%d", &n);
    
    int *arr = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }
    
    // ソート
    qsort(arr, n, sizeof(int), compare);
    
    int mode = arr[0];
    int max_count = 1;
    int current_count = 1;
    
    for (int i = 1; i < n; i++) {
        if (arr[i] == arr[i-1]) {
            current_count++;
        } else {
            if (current_count > max_count) {
                max_count = current_count;
                mode = arr[i-1];
            }
            current_count = 1;
        }
    }
    
    // 最後の要素のカウントをチェック
    if (current_count > max_count) {
        mode = arr[n-1];
    }
    
    printf("%d\n", mode);
    
    free(arr);
    return 0;
}