#include <stdio.h>
#include <string.h>

void print_min_add_palindrome(const char *s);

int main(int argc, char *argv[]) {
    if (argc < 2) return 1;
    char *s = argv[1];
    int len = strlen(s);
    if (len > 100) return 1;
    int is_palindrome = 1;
    for (int i = 0; i < len / 2; i++) {
        if (s[i] != s[len - 1 - i]) {
            is_palindrome = 0;
            break;
        }
    }
    printf("%s\n", is_palindrome ? "YES" : "NO");
    // 回文でない場合はreturnせず、次の処理へ進む
    if (!is_palindrome) {
        printf("%s",s);
        print_min_add_palindrome(s);
    }
    return 0;
}
// 文字列sを最小の追加で回文にするために末尾に追加する文字列を求める関数
void print_min_add_palindrome(const char *s) {
    int len = strlen(s);
    int add_len = 0;
    for (int i = 1; i < len; i++) {
        int is_pal = 1;
        for (int j = 0; j < len - i; j++) {
            if (s[i + j] != s[len - 1 - j]) {
                is_pal = 0;
                break;
            }
        }
        if (is_pal) {
            add_len = i;
            break;
        }
    }
    if (add_len == 0) add_len = len;
    for (int i = add_len - 1; i >= 0; i--) {
        putchar(s[i]);
    }
    printf("\n");
}

