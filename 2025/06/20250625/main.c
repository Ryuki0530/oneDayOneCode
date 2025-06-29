#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 1024

void str_case_change(char *str);

int main(void){
    FILE *input_fp;
    FILE *output_fp;
    char buffer[BUFFER_SIZE];
    size_t len;
    input_fp = fopen("input.txt", "r");
    output_fp = fopen("output.txt", "w");
    
    if (input_fp == NULL || output_fp == NULL) {
        perror("Error opening file");
        return(1);
    }

    while (fgets(buffer, sizeof(buffer), input_fp) != NULL) {
        len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0'; // Remove newline character
        }
        str_case_change(buffer);
        fprintf(output_fp, "%s\n", buffer);
    }

    printf("File processing completed successfully.\n");
    fclose(input_fp);
    fclose(output_fp);

    return(0);
}

void str_case_change(char *str){
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= 32; // Convert to uppercase
        } else if (*str >= 'A' && *str <= 'Z') {
            *str += 32; // Convert to lowercase
        }
        str++;
    }
}
