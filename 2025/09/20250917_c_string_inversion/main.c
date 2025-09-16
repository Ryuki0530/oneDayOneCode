#include <stdio.h>
#include <string.h>

int main(void) {
    char str[101];
    if (fgets(str, sizeof(str), stdin) == NULL) return 1;

    // Remove newline if present
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
        len--;
    }

    for (int i = (int)len - 1; i >= 0; i--) {
        putchar(str[i]);
    }
    putchar('\n');
    return 0;
}