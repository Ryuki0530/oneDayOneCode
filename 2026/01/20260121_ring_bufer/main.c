#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOG_LENGTH 201

int main(void) {
    int N, M;
    scanf("%d %d", &N, &M);
    getchar();  // 改行を消費
    
    // リングバッファをmallocで確保
    char **buf = (char **)malloc(N * sizeof(char *));
    for (int i = 0; i < N; i++) {
        buf[i] = (char *)malloc(MAX_LOG_LENGTH);
    }
    
    int head = 0;
    
    // M件のログを読み込む
    for (int i = 0; i < M; i++) {
        fgets(buf[head], MAX_LOG_LENGTH, stdin);
        // 改行を削除
        size_t len = strlen(buf[head]);
        if (len > 0 && buf[head][len - 1] == '\n') {
            buf[head][len - 1] = '\0';
        }
        head = (head + 1) % N;
    }
    
    // 出力する件数、開始位置を算出
    int count = (M < N) ? M : N;
    int start = (head - count + N) % N;
    
    // 古いものから新しいものへ順に出力
    for (int i = 0; i < count; i++) {
        int idx = (start + i) % N;
        printf("%s\n", buf[idx]);
    }
    
    //　お片付け
    for (int i = 0; i < N; i++) {
        free(buf[i]);
    }
    free(buf);
    
    return 0;
}