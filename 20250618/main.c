#include <stdio.h>
#include <stdbool.h>

int main() {
    int input;
    scanf("%d", &input);
    for (int i = input; i > 0; i--) {

        bool bool_fizz = (i % 3 == 0);
        bool bool_buzz = (i % 5 == 0);

        if (bool_fizz && bool_buzz) {
            printf("FizzBuzz ");
        } else if (bool_fizz) {
            printf("Fizz ");
        } else if (bool_buzz) {
            printf("Buzz ");
        } else {
            printf("%d ", i);
        }

    }

    // //debug
    // printf("input: %d\n", input);
    
    printf("\n");
    return 0;
}