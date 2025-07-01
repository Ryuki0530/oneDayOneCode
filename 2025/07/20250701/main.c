#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define BUF_SIZE 1024
#define WORD_MAX 1000
#define WORD_LEN 64
#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"

typedef struct{
    char word[WORD_LEN];
    int count;
} word_count;

void to_lower_str(char *word);
int find_word(const char *word);
void add_or_increment_word(const char *word);

char buf[BUF_SIZE];
word_count words[WORD_MAX];
int word_num = 0;

void to_lower_str(char *word) {
    for (; *word; ++word) {
        *word = tolower(*word);
    }
}

int find_word(const char *word) {
    for (int i = 0; i< word_num; i++){
        if(strcmp(words[i].word, word) == 0)return i;
    }
    return -1; // 見つからなければ-1を返す
}

void add_or_increment_word(const char *word) {
    int idx = find_word(word);
    if (idx >= 0) {
        words[idx].count++; // 既存単語ならカウントを増やす
    } else if (word_num < WORD_MAX) {
        strcpy(words[word_num].word, word); // 新規単語を登録
        words[word_num].count = 1;
        word_num++;
    }
}

int cmp_word(const void *a, const void *b) {
    return strcmp(((word_count*)a)->word, ((word_count*)b)->word);
}


int main (void){
    FILE *input_file = fopen(INPUT_FILE, "r");
    if(input_file == NULL){
        printf("ERROR: Could not open %s\n", INPUT_FILE);
        perror("fopen");
        return 1;
    }

    FILE *output_file = fopen(OUTPUT_FILE, "w");
    if(output_file == NULL){
        printf("ERROR: Could not open %s\n", OUTPUT_FILE);
        perror("fopen");
        return 1;
    }

    while(fgets(buf, BUF_SIZE, input_file)){
        char *p = buf;
        while (*p){
            while (*p && !isalpha(*p) && !isdigit(*p))p++;
            if(!*p)break;
            char word[WORD_LEN] = {0};
            int i = 0;
            while(*p && (isalpha(*p) || isdigit(*p)) && i < WORD_LEN - 1){
                word[i++] = tolower(*p);
                p++;
            }
            word[i] = '\0'; // 文字列終端

            int digit_flag = 0;
            for (int k = 0;k < i;k++){
                if (isdigit(word[k])){
                    digit_flag = 1;
                    break;
                }
            }

            if (i > 0 && !digit_flag){
                to_lower_str(word);
                add_or_increment_word(word);
            }
        }
    }
    fclose(input_file);
    qsort(words, word_num, sizeof(word_count), cmp_word);

    for (int i = 0; i < word_num; i++) {
        fprintf(output_file, "%s: %d\n", words[i].word, words[i].count);
    }

    fclose(output_file);
    return 0;
}