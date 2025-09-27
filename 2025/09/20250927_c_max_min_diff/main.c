#define MAX_INPUT_SIZE 100
#define INPUT_MINIMUM -10000
#define INPUT_MAXIMUM 10000
#include <stdio.h>

int main() {
    int numbers[MAX_INPUT_SIZE];
    int count = 0;
    int current_input;
    
    while (count < MAX_INPUT_SIZE && scanf("%d", &current_input) == 1) {
        if (current_input < INPUT_MINIMUM || current_input > INPUT_MAXIMUM) {
            printf("Invalid input element\n");
            return 1;
        }
        numbers[count] = current_input;
        count++;
        if (getchar() == '\n') break; // Stop reading on newline
    }
    
    if (count == 0) {
        printf("No valid input\n");
        return 1;
    }
    
    int max = numbers[0];
    int min = numbers[0];
    
    for (int i = 1; i < count; i++) {
        if (numbers[i] > max) max = numbers[i];
        if (numbers[i] < min) min = numbers[i];
    }
    
    printf("%d\n", max - min);
    
    return 0;
}