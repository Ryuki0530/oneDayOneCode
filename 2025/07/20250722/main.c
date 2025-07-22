#include<stdio.h>
#define MAX_INPUT 100
int main (void){
    int sum = 0;
    int i = 0;
    for (i = 0; i < MAX_INPUT; i++) {
        int input;
        if (scanf("%d", &input) != 1) {
            break; // Exit loop on invalid input
        }
        if (input <= 0) {
            break; // Exit loop on negative input
        }
        sum += input;
    }
    printf("‡Œv: %d\n", sum);
    printf("•½‹Ï: %.2f\n", (i > 0) ? (float)sum / i : 0.0);
    return 0;
}