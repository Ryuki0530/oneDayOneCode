#include <stdio.h>
#include <string.h>

int main() {
    char str[101];
    fgets(str, sizeof(str), stdin);

    // Remove newline character from the end if present
    str[strcspn(str, "\n")] = 0;

    int len = strlen(str);
    for (int i = len - 1; i >= 0; i--) {
        putchar(str[i]);
    }
    putchar('\n');

    return 0;
}
