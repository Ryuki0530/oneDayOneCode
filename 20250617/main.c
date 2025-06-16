#include <stdio.h>
#define INPUT_STR_LEN 1000

int main() {
    char str[INPUT_STR_LEN];
    char prev_char = '\0';
    int count = 0;

    fgets(str, sizeof(str), stdin);

    // ランレングス圧縮
    for(int i = 0; str[i] != '\0'; i++) {
        char current_char = str[i];
        if(current_char == '\n') break;

        if(prev_char == '\0') {
            prev_char = current_char;
            count = 1;
        } else if(prev_char == current_char) {
            count++;
        } else {
            printf("%c%d", prev_char, count);
            prev_char = current_char;
            count = 1;
        }
    }
    // 最後の文字の処理
    if(prev_char != '\0' ) {
        printf("%c%d", prev_char, count);
    }

    printf("\n");
    //debug
    // printf("入力文字列: %s\n", str);
    return 0;
}