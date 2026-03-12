#include <stdio.h>

int main(void) {

    
    int N;
    int a[100];
    scanf("%d", &N);

    for (int i = 0; i < N; i++) {
        scanf("%d", &a[i]);
    }

    int first = 1;
    for (int i = 0; i < N; i++) {
        int is_duplicate = 0;
        for (int j = 0; j < i; j++) {
            if (a[i] == a[j]) {
                is_duplicate = 1;
                break;
            }
        }
        if (!is_duplicate) {
            if (!first)printf(" ");
            printf("%d", a[i]);
            first = 0;
        }
    }

    printf("\n");
    return 0;
}