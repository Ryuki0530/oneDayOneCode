#include<stdio.h>
#include<string.h>

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100
#define MAX_WORDS_LENGTH 100

int word_parser(const char *text, char words[MAX_WORDS][MAX_WORD_LENGTH], const char *delims) {
    char buf[MAX_WORDS_LENGTH * MAX_WORD_LENGTH];
    int count = 0;
    char *token;

    strncpy(buf, text, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    token = strtok(buf, delims);
    while (token != NULL && count < MAX_WORDS) {
        strncpy(words[count], token, MAX_WORD_LENGTH - 1);
        words[count][MAX_WORD_LENGTH - 1] = '\0';
        count++;
        token = strtok(NULL, delims);
    }
    return count;
}

int main (void){

    char input[MAX_WORDS_LENGTH];
    char words[MAX_WORDS][MAX_WORD_LENGTH];

    printf("Enter Word: ");
    fgets(input, sizeof(input), stdin);
    int word_count = word_parser(input, words, " \n");
    for (int i = 0; i < word_count; i++) {
        printf("Word %d: %s\n", i + 1, words[i]);
    }
    return 0;
}