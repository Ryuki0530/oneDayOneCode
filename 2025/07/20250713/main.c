#include <stdio.h>
#define FILE_PATH "scores.csv"
#define MAX_RECORDS 100

typedef struct {
    int id;
    char name[50];
    int japanese_score;
    int math_score;
    int english_score;
} record;

int read_score_file(const char* file_path , record* records, int max_records) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL){
        perror("Failed to open file");
        return -1;
    }
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        perror("Failed to read header line");
        return -1;
    }
    int count = 0;
    while(count < max_records && fscanf(file, "%d,%49[^,],%d,%d,%d",
        &records[count].id, 
        records[count].name,
        &records[count].japanese_score,
        &records[count].math_score,
        &records[count].english_score) == 5
    ) {
        count++;
    }

    fclose(file);
    return count;
}

double calculate_average(int japanese_score, int math_score, int english_score) {
    return (japanese_score + math_score + english_score) / 3.0;
}

int main(){

    record records[MAX_RECORDS];
    int count = read_score_file(FILE_PATH, records, MAX_RECORDS);
    if (count == -1) {
        return 1;
    }

    for (int i = 0; i < count; i++) {
        printf("ID: %d, 名前: %s, 平均点: %.2f\n",
            records[i].id,
            records[i].name,
            calculate_average(records[i].japanese_score, records[i].math_score, records[i].english_score));
    }
    return 0;
}