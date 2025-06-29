#include <stdio.h>
int char_index(char c);

int main() {
    char str[1000];
    fgets(str, sizeof(str), stdin);

    int count[26] = {0};
    int order[26] = {0};
    int currentOrder = 1;

    int prevIndex = -1;
    int currentContinuous = 1;

    for(int i = 0; str[i] != '\0'; i++) {
        int idx = char_index(str[i]);
        if (idx != -1) {
            count[idx]++;
            if (order[idx] == 0) {
                order[idx] = currentOrder;
                currentOrder++;
            }
            if (prevIndex == idx) {
                currentContinuous++;
            } else {
                if (prevIndex != -1 /*&& currentContinuous > 1*/) {
                    printf("%c: %d \n", 'a' + prevIndex, currentContinuous);
                }
                currentContinuous = 1;
            }
            prevIndex = idx;
        }
    }
    
// ループ終了後、最後の連続部分を出力
    if (/*currentContinuous > 1 &&*/ prevIndex != -1) {
        printf("%c: %d \n", 'a' + prevIndex, currentContinuous);
    }
    // //debug
    // printf("order配列の中身: ");
    // for(int i = 0; i < 26; i++) {
    //     printf("%d ", order[i]);
    // }
    // printf("\n");
    // for(int ord = 1; ord < currentOrder; ord++) {
    //     for(int i = 0; i < 26; i++) {
    //         if(order[i] == ord) {
    //             printf("%c: %d ", 'a' + i, count[i]);
    //         }
    //     }
    // }
}

int char_index(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a';
    }
    return -1;
}