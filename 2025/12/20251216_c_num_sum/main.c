#include <stdio.h>
#include <ctype.h>

int main(void) {
    char buffer[128];
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        return 0;
    }

    int sum = 0;
    for (char *p = buffer; *p; ++p) {
        if (isdigit((unsigned char)*p)) {
            sum += *p - '0';
        }
    }

    printf("%d\n", sum);
    return 0;
}