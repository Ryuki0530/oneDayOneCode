#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define COLUMN_INDEX 1

typedef enum {
    SUCCESS,
    FILE_NOT_FOUND,
    READ_ERROR
} error_t;

// prototypes
FILE * open_file(const char *filename, error_t *error);
void read_line(FILE *file, char *buffer, size_t size, error_t *error);
int add_csv_columns(FILE *file, int column, error_t *error);
int extract_column_value(const char *line, int column, error_t *error);


FILE * open_file(const char *filename , error_t *error) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        *error = FILE_NOT_FOUND;
    }
    else {
        *error = SUCCESS;
    }
    return file;
}

void read_line(FILE *file, char *buffer, size_t size , error_t *error) {
    if (fgets(buffer, size, file) != NULL) {
        *error = SUCCESS;
        return;
    }
    *error = READ_ERROR;
    return;
}

int add_csv_columns(FILE *file, int column , error_t *error) {
    int sum = 0;
    char buffer[BUFFER_SIZE];
    error_t line_error;

    while (1) {
        read_line(file, buffer, sizeof(buffer), &line_error);
        if (line_error != SUCCESS) {
            break; // ファイルの終端またはエラーでループを抜ける
        }
        
        // CSVの各行を処理し、指定列の値を加算
        error_t extract_error;
        int value = extract_column_value(buffer, column, &extract_error);
        if (extract_error != SUCCESS) {
            // エラー処理
            *error = READ_ERROR;
            return sum;
        }
        sum += value;
    }
    *error = SUCCESS; // 成功を設定
    return sum;
}

int extract_column_value(const char *line, int column , error_t *error) {
    int current_column = 0;
    const char *ptr = line;
    char value[64];
    int value_index = 0;

    while (*ptr != '\0' && *ptr != '\n') {
        if (*ptr == ',') {
            if (current_column == column) {
                value[value_index] = '\0'; // 終端文字を追加
                *error = SUCCESS; // 成功を設定
                return atoi(value); // 文字列を整数に変換して返す
            }
            current_column++;
            value_index = 0; // 次の値のためにインデックスをリセット
        } else {
            if (value_index < sizeof(value) - 1) { // バッファオーバーフローを防ぐ
                value[value_index++] = *ptr;
            }
        }
        ptr++;
    }

    // 最後の列の処理
    if (current_column == column) {
        value[value_index] = '\0';
        *error = SUCCESS;
        return atoi(value);
    }

    *error = READ_ERROR;
    return 0; // 指定列が存在しない場合は0を返す
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int column = COLUMN_INDEX;
    error_t error;

    FILE *file = open_file(filename, &error);
    if (error != SUCCESS) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }

    int sum = add_csv_columns(file, column, &error);
    fclose(file);

    if (error != SUCCESS) {
        fprintf(stderr, "Error reading file.\n");
        return 1;
    }

    printf("Sum of column %d: %d\n", column, sum);
    return 0;
}