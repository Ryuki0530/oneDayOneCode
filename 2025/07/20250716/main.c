#include <stdio.h>
#define INPUT_VALUE 5

int main() {
    int current_input;
    int current_min;
    int current_max;
    for(int i = 0; i < INPUT_VALUE; i++ ){
        scanf("%d",&current_input);
        if(i == 0){
            current_max = current_input;
            current_min = current_input;
            continue;
        }
        if(current_input > current_max) current_max = current_input;
        if(current_input < current_min) current_min = current_input;
    }
    printf("最大値 :%d\n最小値 :%d\n",current_max,current_min);
    return 0;
}