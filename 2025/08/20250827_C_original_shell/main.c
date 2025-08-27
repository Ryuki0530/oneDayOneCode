#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdarg.h>
#include <fcntl.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS     128

static char g_api_key[256] = {0};

/* ========= 翻訳モード関連 ここから (統合版) =========
   実際の翻訳ロジックは translation_core() のみを編集してください。
   translate_text / translate_chunk は共通コアを呼ぶ薄いラッパです。
*/
static bool g_translate_mode = false;

/* 共通コア: src (長さlen) を翻訳して新規malloc文字列を返す
   has_final_nl: 末尾に改行を戻すか
   kind: 0=行/内部メッセージ, 1=チャンク
   TODO: ここを書き換えて本当の翻訳を実装
*/
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

/* 行単位/内部メッセージ翻訳 */
char *translate_text(const char *src) {
    if (!src) return NULL;
    if (!g_translate_mode) return strdup(src);

    size_t len = strlen(src);
    bool has_nl = (len > 0 && src[len-1] == '\n');
    if (has_nl) len--; /* 改行除いてコア処理 */

    return translation_core(src, len, has_nl, 0);
}

/* チャンク翻訳: ストリーム断片
   out_len には返却文字列のバイト長(終端は含めない)を格納
*/
char *translate_chunk(const char *src, size_t len, size_t *out_len) {
    if (out_len) *out_len = 0;
    if (!src) return NULL;

    if (!g_translate_mode) {
        char *raw = malloc(len + 1);
        if (!raw) return NULL;
        memcpy(raw, src, len);
        raw[len] = '\0';
        if (out_len) *out_len = len;
        return raw;
    }

    /* チャンクは改行保証しないので has_final_nl=false でコア呼び出し */
    char *tr = translation_core(src, len, false, 1);
    if (tr && out_len) *out_len = strlen(tr); /* プレ/ポスト含む */
    return tr;
}

/* シェル内部メッセージ出力用ラッパ */
static void shell_vprintf(const char *fmt, va_list ap) {
    char tmp[4096];
    int n = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    if (n < 0) return;

    if ((size_t)n < sizeof(tmp)) {
        char *tr = translate_text(tmp);
        if (tr) {
            fputs(tr, stdout);
            free(tr);
        }
    } else {
        size_t need = (size_t)n + 1;
        char *dyn = malloc(need);
        if (!dyn) return;
        vsnprintf(dyn, need, fmt, ap);
        char *tr = translate_text(dyn);
        if (tr) {
            fputs(tr, stdout);
            free(tr);
        }
        free(dyn);
    }
}

static void shell_printf(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    shell_vprintf(fmt, ap);
    va_end(ap);
}

static void shell_eprintf(const char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    char tmp[4096];
    int n = vsnprintf(tmp, sizeof(tmp), fmt, ap);
    va_end(ap);
    if (n < 0) return;
    char *tr = translate_text(tmp);
    if (tr) {
        fputs(tr, stderr);
        free(tr);
    }
}
/* ========= 翻訳モード関連 統合版 ここまで ========= */

// Prototypes
int  main_loop(bool *exit_flag);
char **split_string(const char *str, char delim);
void free_string_array(char **arr);
char *trim(char *s);
char **parse_args(const char *cmd);
void free_args(char **args);
int execute_pipeline(char *line);

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--t") == 0) {
            g_translate_mode = true;
            // 次の引数がAPIキーなら保存
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                strncpy(g_api_key, argv[i + 1], sizeof(g_api_key) - 1);
                g_api_key[sizeof(g_api_key) - 1] = '\0';
                i++; // APIキー分スキップ
            }
        }
    }
    if (g_translate_mode) {
        fprintf(stdout, "[translation mode ON]\n");
        if (g_api_key[0]) {
            fprintf(stdout, "[API key set]\n");
        }
    }
    bool exit_flag = false;
    main_loop(&exit_flag);
    return 0;
}

int main_loop(bool *exit_flag) {
    char input[MAX_INPUT_SIZE];
    shell_printf("Enter commands (separated by ';', type 'exit' to quit):\n");
    while (!(*exit_flag)) {
        shell_printf("> ");
        if (!fgets(input, sizeof(input), stdin)) {
            shell_printf("\n");
            break;
        }
        input[strcspn(input, "\n")] = '\0';

        char **commands = split_string(input, ';');
        if (!commands) continue;

        for (size_t i = 0; commands[i]; ++i) {
            char *cmdline = trim(commands[i]);
            if (*cmdline == '\0') continue;
            if (strcmp(cmdline, "exit") == 0) {
                *exit_flag = true;
                break;
            }
            int status = execute_pipeline(cmdline);
            if (status != 0) {
                shell_eprintf("[exit=%d]\n", status);
            }
        }
        free_string_array(commands);
    }
    return 0;
}

// 文字列分割
char **split_string(const char *str, char delim) {
    if (!str) return NULL;

    size_t count = 1;
    for (const char *p = str; *p; ++p)
        if (*p == delim) count++;

    char **result = malloc((count + 1) * sizeof(char *));
    if (!result) return NULL;

    size_t idx = 0;
    const char *start = str;
    const char *p = str;
    while (1) {
        if (*p == delim || *p == '\0') {
            size_t len = (size_t)(p - start);
            char *frag = malloc(len + 1);
            if (!frag) {
                for (size_t j = 0; j < idx; ++j) free(result[j]);
                free(result);
                return NULL;
            }
            memcpy(frag, start, len);
            frag[len] = '\0';
            result[idx++] = frag;
            if (*p == '\0') break;
            start = p + 1;
        }
        p++;
    }
    result[idx] = NULL;
    return result;
}

void free_string_array(char **arr) {
    if (!arr) return;
    for (size_t i = 0; arr[i]; ++i) free(arr[i]);
    free(arr);
}

// 前後空白除去
char *trim(char *s) {
    while (*s && (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n')) s++;
    if (*s == '\0') return s;
    char *end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) {
        *end-- = '\0';
    }
    return s;
}

// コマンドを空白でトークン化
char **parse_args(const char *cmd) {
    char **args = calloc(MAX_TOKENS, sizeof(char *));
    if (!args) return NULL;

    char *work = strdup(cmd);
    if (!work) { free(args); return NULL; }

    size_t idx = 0;
    char *token = strtok(work, " \t");
    while (token && idx < MAX_TOKENS - 1) {
        args[idx++] = strdup(token);
        token = strtok(NULL, " \t");
    }
    args[idx] = NULL;
    free(work);
    return args;
}

void free_args(char **args) {
    if (!args) return;
    for (size_t i = 0; args[i]; ++i) free(args[i]);
    free(args);
}

// パイプライン実行 (翻訳モード時は最終出力を捕捉して翻訳)
int execute_pipeline(char *line) {
    char **stages = split_string(line, '|');
    if (!stages) return -1;

    size_t count = 0;
    for (; stages[count]; ++count);
    if (count == 0) {
        free_string_array(stages);
        return 0;
    }

    int prev_read = -1;
    int capture_fd = -1; // 翻訳用(最終段のstdout)
    pid_t *pids = malloc(sizeof(pid_t) * count);
    if (!pids) {
        free_string_array(stages);
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        char *stage = trim(stages[i]);
        if (*stage == '\0') {
            shell_eprintf("空のパイプセクション\n");
            free(pids);
            free_string_array(stages);
            return -1;
        }

        int pipefd[2] = {-1,-1};
        bool need_pipe = (i < count - 1) || (g_translate_mode && i == count - 1);
        if (need_pipe) {
            if (pipe(pipefd) < 0) {
                perror("pipe");
                free(pids);
                free_string_array(stages);
                return -1;
            }
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            if (pipefd[0] != -1) { close(pipefd[0]); close(pipefd[1]); }
            free(pids);
            free_string_array(stages);
            return -1;
        }

        if (pid == 0) { // child
            if (prev_read != -1) {
                if (dup2(prev_read, STDIN_FILENO) < 0) { perror("dup2"); _exit(1); }
            }
            if (need_pipe) {
                // 自分のstdoutをpipeへ
                close(pipefd[0]);
                if (dup2(pipefd[1], STDOUT_FILENO) < 0) { perror("dup2"); _exit(1); }
            }
            if (prev_read != -1) close(prev_read);
            if (need_pipe) close(pipefd[1]);

            char **args = parse_args(stage);
            if (!args || !args[0]) {
                shell_eprintf("実行不可: %s\n", stage);
                free_args(args);
                _exit(127);
            }
            execvp(args[0], args);
            perror("execvp");
            free_args(args);
            _exit(127);
        } else { // parent
            pids[i] = pid;
            if (prev_read != -1) close(prev_read);
            if (need_pipe) {
                close(pipefd[1]);
                if (g_translate_mode && i == count - 1) {
                    capture_fd = pipefd[0];
                } else {
                    prev_read = pipefd[0];
                }
            }
        }
    }

    if (prev_read != -1) close(prev_read);

    // 最終段出力を翻訳モードで捕捉
    if (g_translate_mode && capture_fd != -1) {
        char buf[4096];
        ssize_t n;
        while ((n = read(capture_fd, buf, sizeof(buf))) > 0) {
            size_t out_len = 0;
            char *tr = translate_chunk(buf, (size_t)n, &out_len);
            if (tr) {
                if (out_len > 0) fwrite(tr, 1, out_len, stdout);
                free(tr);
            }
        }
        close(capture_fd);
        fflush(stdout);
    }

    int last_status = 0;
    for (size_t i = 0; i < count; ++i) {
        int wstatus;
        if (waitpid(pids[i], &wstatus, 0) < 0) {
            perror("waitpid");
            last_status = -1;
        } else {
            if (WIFEXITED(wstatus))
                last_status = WEXITSTATUS(wstatus);
            else
                last_status = -1;
        }
    }

    free(pids);
    free_string_array(stages);
    return last_status;

}