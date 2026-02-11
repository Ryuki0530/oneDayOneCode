#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 201

static void die(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

int main(void) {
    int N, M;

    if (scanf("%d %d", &N, &M) != 2) {
        die("Failed to read N M");
    }

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }

    if (N <= 0 || M < 0) {
        die("Invalid N or M");
    }

    char (*buf)[MAX_LINE] = NULL;
    buf = (char (*)[MAX_LINE])malloc(sizeof(char[MAX_LINE]) * (size_t)N);
    if (!buf) {
        die("malloc failed");
    }

    int head = 0;
    int count = 0;

    for (int i = 0; i < M; i++) {
        if (!fgets(buf[head], MAX_LINE, stdin)) {
            break;
        }


        head = (head + 1) % N;
        if (count < N) {
            count++;
        }
    }

    int start;
    if (count < N) {
        start = 0;
    } else {
        start = head;
    }

    for (int i = 0; i < count; i++) {
        int idx = (start + i) % N;
        fputs(buf[idx], stdout);
    }

    free(buf);
    return 0;
}