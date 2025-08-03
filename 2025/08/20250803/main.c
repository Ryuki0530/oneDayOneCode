#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 10000
#define MAX_WORD_LENGTH 100

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

// prototypes
FILE* open_file(const char* path, const char* mode);
char* read_line(FILE* fp);
void close_file(FILE* fp);
void count_words_in_line(char* line, WordCount* word_counts, int* num_words);
int find_word(WordCount* word_counts, int num_words, const char* word);
void print_word_counts(WordCount* word_counts, int num_words);

int main(int argc, char* argv[]) {
    bool debug = false;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_path> [--Debug]\n", argv[0]);
        return 1;
    }
    if (argc >= 3 && strcmp(argv[2], "--Debug") == 0) {
        debug = true;
    }
    if (debug) {
        printf("Debug mode is ON\n");
    }
    
    const char* file_path = argv[1];
    FILE* file = open_file(file_path, "r");
    if (file == NULL) {
        return 1;
    }

    WordCount word_counts[MAX_WORDS];
    int num_words = 0;

    char* line;
    while ((line = read_line(file)) != NULL) {
        if (debug) {
            printf("Read line: %s", line);
        }
        count_words_in_line(line, word_counts, &num_words);
    }
    
    close_file(file);
    printf("\n");
    print_word_counts(word_counts, num_words);
    
    return 0;
}

FILE* open_file(const char* path, const char* mode) {
    FILE* fp = fopen(path, mode);
    if (fp == NULL) {
        perror("Failed to open file");
    }
    return fp;
}

char* read_line(FILE* fp) {
    static char buffer[1024];
    if (fp == NULL) return NULL;
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        return buffer;
    }
    return NULL;
}

void close_file(FILE* fp) {
    if (fp != NULL) {
        fclose(fp);
    }
}

void count_words_in_line(char* line, WordCount* word_counts, int* num_words) {
    char* token = strtok(line, " \t\n\r\f\v.,!?;:\"'()[]{}");
    
    while (token != NULL && *num_words < MAX_WORDS) {
        // 小文字に変換
        for (int i = 0; token[i]; i++) {
            token[i] = tolower(token[i]);
        }
        
        // 既存の単語かチェック
        int index = find_word(word_counts, *num_words, token);
        if (index != -1) {
            word_counts[index].count++;
        } else {
            // 新しい単語を追加
            strncpy(word_counts[*num_words].word, token, MAX_WORD_LENGTH - 1);
            word_counts[*num_words].word[MAX_WORD_LENGTH - 1] = '\0';
            word_counts[*num_words].count = 1;
            (*num_words)++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v.,!?;:\"'()[]{}");
    }
}

int find_word(WordCount* word_counts, int num_words, const char* word) {
    for (int i = 0; i < num_words; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void print_word_counts(WordCount* word_counts, int num_words) {
    for (int i = 0; i < num_words; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}
