#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* プロトタイプ宣言 */
static char *translation_core(const char *src, size_t len, bool has_final_nl, int kind);

static char g_api_key[256] = {0};


// static char *translation_core(const char *src, size_t len, bool has_final_nl, int kind) {
//     /* 例: 小文字→大文字 + 共通プレフィックス/サフィックス */
//     const char *pre  = (kind == 0) ? "[TR]" : "[TR]";
//     const char *post = (kind == 0) ? "[/TR]" : "[/TR]";
//     size_t prelen = strlen(pre);
//     size_t postlen = strlen(post);

//     /* 変換後バッファ確保 */
//     char *buf = malloc(prelen + len + postlen + (has_final_nl ? 1 : 0) + 1);
//     if (!buf) return NULL;

//     memcpy(buf, pre, prelen);
//     for (size_t i = 0; i < len; ++i) {
//         unsigned char c = (unsigned char)src[i];
//         if ('a' <= c && c <= 'z') c = (unsigned char)(c - 'a' + 'A'); /* 簡易置換 */
//         buf[prelen + i] = (char)c;
//     }
//     memcpy(buf + prelen + len, post, postlen);
//     size_t pos = prelen + len + postlen;
//     if (has_final_nl) buf[pos++] = '\n';
//     buf[pos] = '\0';
//     return buf;
// }

struct MemoryStruct {
    char *memory;
    size_t size;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr) return 0;

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;
    return realsize;
}


static char *translation_core(const char *src, size_t len, bool has_final_nl, int kind) {
    char cmd[2048] = {0};
    snprintf(cmd, sizeof(cmd), "python3 translate.py \"%.*s\" \"%s\"", (int)len, src, g_api_key);

    int ret = system(cmd);
    if (ret != 0) return NULL;

    FILE *fp = fopen("result.txt", "r");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    long flen = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *buf = malloc(flen + 32);
    if (!buf) { fclose(fp); return NULL; }

    fread(buf, 1, flen, fp);
    fclose(fp);

    // wrap with tags
    char *result = malloc(flen + 32);
    if (!result) return NULL;

    snprintf(result, flen + 32, "[JP]%s%s[/JP]%s", buf, has_final_nl ? "\n" : "", "");

    free(buf);
    return result;
}

int main(void) {
    const char *input = "hello, world!";
    size_t len = strlen(input);

    /* 改行あり・なし、kind=0/1 でテスト */
    for (int kind = 0; kind <= 1; ++kind) {
        for (int nl = 0; nl <= 1; ++nl) {
            char *result = translation_core(input, len, nl, kind);
            if (result) {
                printf("kind=%d, has_final_nl=%d: \"%s\"\n", kind, nl, result);
                free(result);
            } else {
                fprintf(stderr, "translation_core failed\n");
            }
        }
    }
    return 0;
}