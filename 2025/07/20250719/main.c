#include <stdio.h>
#include <stdlib.h>

#define MAX_NUMS 100

int compare(const void *a, const void *b) {
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    return (int_a > int_b) - (int_a < int_b);
}

int main() {
    int nums[MAX_NUMS];
    int count = 0;
    
    while (scanf("%d", &nums[count]) == 1) {
        count++;
        if (count >= MAX_NUMS) {
            break;
        }
    }
    printf("\n\n");
    qsort(nums, count, sizeof(int), compare);

    for (int i = 0; i < count; i++) {
        if (i > 0) printf(" ");
        printf("%d", nums[i]);
    }
    printf("\n");

    return 0;
}