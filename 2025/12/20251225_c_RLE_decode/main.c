#include <stdio.h>
#include <ctype.h>

int main(void) {
    int ch;
    while ((ch = getchar()) != EOF) {
        if (ch == '\n') break;
        if (ch == '\r') continue;

        unsigned long long count = 0;
        int d;
        while ((d = getchar()) != EOF && isdigit(d)) {
            count = count * 10 + (unsigned long long)(d - '0');
        }

        for (unsigned long long i = 0; i < count; ++i) {
            putchar(ch);
        }

        if (d == EOF) {
            putchar('\n');
            return 0;
        }
        if (d == '\n') {
            putchar('\n');
            return 0;
        }
        if (d == '\r') {
            continue;
        }
        ungetc(d, stdin);
    }

    putchar('\n');
    return 0;
}