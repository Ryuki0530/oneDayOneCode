#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define FILE_PATH "books.txt"
#define MAX_LINE_LENGTH 256
#define MAX_TITLE_LENGTH 100
#define MAX_AUTHOR_LENGTH 100

typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    char author[MAX_AUTHOR_LENGTH];
    int stock;
} Book;

int load_books_data(const char* file_path, Book books[], int max_books) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int count = 0;

    while (fgets(line, sizeof(line), file) && count < max_books) {
        line[strcspn(line, "\n")] = '\0';
        
        char *token = strtok(line, ",");
        if (token != NULL) {
            books[count].id = atoi(token);
            
            token = strtok(NULL, ",");
            if (token != NULL) {
                strcpy(books[count].title, token);
                
                token = strtok(NULL, ",");
                if (token != NULL) {
                    strcpy(books[count].author, token);
                    
                    token = strtok(NULL, ",");
                    if (token != NULL) {
                        books[count].stock = atoi(token);
                        count++;
                    }
                }
            }
        }
    }

    fclose(file);
    return count;
}

int search_book_by_title(const Book books[], int book_count, const char* title) {
    for (int i = 0; i < book_count; i++) {
        if (strcmp(books[i].title, title) == 0) {
            return i;
        }
    }
    return -1;
}
void display_book_stock(const Book* book) {
    printf("書籍名: %s\n", book->title);
    printf("著者: %s\n", book->author);
    printf("在庫数: %d\n", book->stock);
}

void get_title_input(char* title) {
    printf("検索する書籍のタイトルを入力してください: ");
    fgets(title, MAX_TITLE_LENGTH, stdin);
    title[strcspn(title, "\n")] = '\0';
}

int main() {
    Book books[100];
    char search_title[MAX_TITLE_LENGTH];
    
    int book_count = load_books_data(FILE_PATH, books, 100);
    if (book_count == -1) {
        printf("書籍データの読み込みに失敗しました。\n");
        return 1;
    }
    
    printf("書籍データを %d 件読み込みました。\n\n", book_count);
    get_title_input(search_title);
    
    int found_index = search_book_by_title(books, book_count, search_title);
    
    if (found_index != -1) {
        printf("\n書籍が見つかりました:\n");
        display_book_stock(&books[found_index]);
    } else {
        printf("\n書籍が見つかりません\n");
    }
    
    return 0;
}

