#include <stdio.h>

int main(void) {
    int value;
    int current = 0;
    int count = 0;

    while (scanf("%d", &value) == 1) {
        if (count == 0) {
            current = value;
            count = 1;
        } else if (value == current) {
            count++;
        } else {
            printf("%d x %d\n", current, count);
            current = value;
            count = 1;
        }
    }

    if (count > 0) {
        printf("%d x %d\n", current, count);
    }

    return 0;
}