#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#define MAX 1024

bool is_uppercase(char c) {
    return c >= 'A' && c <= 'Z';
}

bool is_lowercase(char c) {
    return c >= 'a' && c <= 'z';
}

char to_lowercase(char c) {
    return c + ('a' - 'A');
}
char to_uppercase(char c) {
    return c - ('a' - 'A');
}

char *str_toggle_case(char *str) {
    for(int i = 0; str[i]; i++) {
        if(is_uppercase(str[i])) {
            str[i] = to_lowercase(str[i]);
        } else if(is_lowercase(str[i])) {
            str[i] = to_uppercase(str[i]);
        } else {
            // Non-alphabetic characters remain unchanged   
        }
    }
    return str;
}

int main() {
    char str[MAX];
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0';
    str_toggle_case(str);
    printf("%s", str);
    return 0;
}
