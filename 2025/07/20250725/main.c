#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 50
#define MAX_INPUT_LENGTH 1000

struct WordCount {
    char word[MAX_WORD_LENGTH];
    int count;
};

// 文字列を小文字に変換する関数
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// 単語リストから指定された単語のインデックスを検索する関数
int findWordIndex(struct WordCount words[], int wordCount, const char *word) {
    for (int i = 0; i < wordCount; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1; // 見つからない場合
}

// 単語をカウントリストに追加または更新する関数
int addOrUpdateWord(struct WordCount words[], int *wordCount, const char *word) {
    int index = findWordIndex(words, *wordCount, word);
    
    if (index != -1) {
        // 既存の単語の場合、カウントを増加
        words[index].count++;
    } else {
        // 新しい単語の場合、リストに追加
        if (*wordCount >= MAX_WORDS) {
            printf("警告: 単語数が上限（%d）に達しました。\n", MAX_WORDS);
            return 0;
        }
        strcpy(words[*wordCount].word, word);
        words[*wordCount].count = 1;
        (*wordCount)++;
    }
    return 1;
}

// 結果を表示する関数
void displayResults(struct WordCount words[], int wordCount) {

    for (int i = 0; i < wordCount; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

// 単語をアルファベット順にソートする関数
void sortWords(struct WordCount words[], int wordCount) {
    struct WordCount temp;
    for (int i = 0; i < wordCount - 1; i++) {
        for (int j = i + 1; j < wordCount; j++) {
            if (strcmp(words[i].word, words[j].word) > 0) {
                temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    char word[MAX_WORD_LENGTH];
    struct WordCount words[MAX_WORDS];
    int wordCount = 0;
    
    
    // 標準入力から1行読み込み
    if (!fgets(input, sizeof(input), stdin)) {
        printf("入力エラーが発生しました。\n");
        return 1;
    }
    
    // 改行文字を削除
    input[strcspn(input, "\n")] = '\0';
    
    // 入力文字列から単語を抽出してカウント
    char *ptr = input;
    int wordIndex = 0;
    
    while (*ptr) {
        // 空白をスキップ
        while (*ptr && isspace(*ptr)) {
            ptr++;
        }
        
        // 単語の開始
        if (*ptr && isalpha(*ptr)) {
            wordIndex = 0;
            
            // アルファベットが続く限り単語として抽出
            while (*ptr && isalpha(*ptr) && wordIndex < MAX_WORD_LENGTH - 1) {
                word[wordIndex++] = *ptr++;
            }
            
            word[wordIndex] = '\0';
            
            // 単語が空でない場合
            if (strlen(word) > 0) {
                // 小文字に変換
                toLowerCase(word);
                
                // 単語をカウントリストに追加または更新
                if (!addOrUpdateWord(words, &wordCount, word)) {
                    break;
                }
            }
        } else {
            // アルファベット以外の文字はスキップ
            ptr++;
        }
    }
    
    if (wordCount == 0) {
        return 0;
    }
    
    // 結果をアルファベット順にソート
    sortWords(words, wordCount);
    
    // 結果を表示
    displayResults(words, wordCount);
    
    return 0;
}

