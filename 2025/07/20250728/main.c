#include <stdio.h>
#include <stdbool.h>
#define INPUT_MAX_SIZE 200

//prototypes
int count_words(const char *str);
bool is_blank(char c);

//Main function
int main() {
    char input[INPUT_MAX_SIZE];
    printf("Enter strings :");
    fgets(input, INPUT_MAX_SIZE, stdin);
    int word_count = count_words(input);
    printf("単語数: %d\n", word_count);
    return 0;
}

//subfunctions
int count_words(const char *str) {
    int count = 0;
    bool in_word = false;

    while (1){
        char current_char = *str++;
        if(current_char == '\0') {
            break;
        }
        if (!is_blank(current_char)){
            if(!in_word){
                in_word = true;
                count ++;
            }
        } else {
            in_word = false;
        }
    }
    return count;
}

bool is_blank(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}