#include <stdio.h>
#include <string.h>

int main(void) {
    static char s[200005];
    if (!fgets(s, sizeof(s), stdin)) return 0;

    // 改行排除
    size_t n = strlen(s);
    if (n && (s[n-1] == '\n' || s[n-1] == '\r')) {
        s[--n] = '\0';
        if (n && s[n-1] == '\r') s[--n] = '\0';
    }

    if (n == 0) { puts(""); return 0; }

    char cur = s[0];
    int cnt = 1;
    for (size_t i = 1; i < n; ++i) {
        if (s[i] == cur) {
            ++cnt;
        } else {
            putchar(cur);
            printf("%d", cnt);
            cur = s[i];
            cnt = 1;
        }
    }
    putchar(cur);
    printf("%d\n", cnt);
    return 0;
}