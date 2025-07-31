#include <stdio.h>
#include <string.h>

int main (){
    char str[1001];
    int count[26] = {0}; // a-zの出現数をカウントする配列

    // 文字列の入力
    fgets(str, sizeof(str), stdin);

    // 文字列を走査して各文字の出現数をカウント
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            count[str[i] - 'a']++;
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            count[str[i] - 'A']++;
        }
    }

    // 出力
    for (int i = 0; i < 26; i++) {
        printf("%c: %d\n", 'a' + i, count[i]);
    }

    return 0;
}