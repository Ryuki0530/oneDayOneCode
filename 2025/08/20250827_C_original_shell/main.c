#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKENS     128

// Prototypes
int  main_loop(bool *exit_flag);
char **split_string(const char *str, char delim);
void free_string_array(char **arr);
char *trim(char *s);
char **parse_args(const char *cmd);
void free_args(char **args);
int execute_pipeline(char *line);

int main(void) {
    bool exit_flag = false;
    main_loop(&exit_flag);
    return 0;
}

int main_loop(bool *exit_flag) {
    char input[MAX_INPUT_SIZE];
    printf("Enter commands (separated by ';', type 'exit' to quit):\n");
    while (!(*exit_flag)) {
        printf("> ");
        if (!fgets(input, sizeof(input), stdin)) {
            printf("\n");
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
                fprintf(stderr, "[exit=%d]\n", status);
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

 // パイプライン実行
int execute_pipeline(char *line) {
    char **stages = split_string(line, '|');
    if (!stages) return -1;

    // ステージ数算出
    size_t count = 0;
    for (; stages[count]; ++count);

    if (count == 0) {
        free_string_array(stages);
        return 0;
    }

    int prev_read = -1;
    pid_t *pids = malloc(sizeof(pid_t) * count);
    if (!pids) {
        free_string_array(stages);
        return -1;
    }

    for (size_t i = 0; i < count; ++i) {
        char *stage = trim(stages[i]);
        if (*stage == '\0') {
            fprintf(stderr, "空のパイプセクション\n");
            free(pids);
            free_string_array(stages);
            return -1;
        }

        int pipefd[2] = {-1,-1};
        if (i < count - 1) {
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
            if (i < count - 1) {
                close(pipefd[0]);
                if (dup2(pipefd[1], STDOUT_FILENO) < 0) { perror("dup2"); _exit(1); }
            }
            // 不要fdクローズ
            if (prev_read != -1) close(prev_read);
            if (i < count - 1) close(pipefd[1]);

            char **args = parse_args(stage);
            if (!args || !args[0]) {
                fprintf(stderr, "実行不可: %s\n", stage);
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
            if (i < count - 1) {
                close(pipefd[1]);
                prev_read = pipefd[0];
            }
        }
    }

    if (prev_read != -1) close(prev_read);

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