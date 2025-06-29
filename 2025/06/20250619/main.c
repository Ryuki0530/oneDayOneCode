#include <stdio.h>
const int months_days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int main() {
    int input_month, input_first_day_of_week;
    scanf("%d %d", &input_month, &input_first_day_of_week);

    if((input_month < 1 || input_month >12)||(input_first_day_of_week<0||input_first_day_of_week>6)) {
        printf("Invalid input\n");
        return 1;
    }

    int current_week = input_first_day_of_week;
    int final_day = months_days[input_month - 1];
    printf("Su Mo Tu We Th Fr Sa\n");
    for(int i = 0; i < current_week; i++) {
        printf("   ");
    }
    for(int i = 1; i <= final_day; i++) {
        printf("%02d ", i);
        current_week++;
        if(current_week == 7) {
            printf("\n");
            current_week = 0;
        }
    }



    // //debug
    // printf("input_month: %d, input_first_day_of_week: %d\n", input_month, input_first_day_of_week);

    printf("\n");
    return 0;
}