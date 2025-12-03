#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main(void) {
    char buf[8192];
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    size_t len = strlen(buf);
    if (len > 0 && buf[len-1] == '\n') buf[--len] = '\0';

    for (size_t i = 0; i < len; ) {
        char ch = buf[i++];
        if (!isupper((unsigned char)ch)) continue;
        int count = 0;
        while (i < len && isdigit((unsigned char)buf[i])) {
            count = count * 10 + (buf[i] - '0');
            i++;
        }
        for (int k = 0; k < count; ++k) putchar(ch);
    }
    putchar('\n');
    return 0;
}