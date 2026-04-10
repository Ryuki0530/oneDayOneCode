#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char line[1024];
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 1;
    }

    char *token = strtok(line, " \n");
    if (token == NULL) {
        return 1;
    }

    int current_value = atoi(token);
    int count = 1;

    while ((token = strtok(NULL, " \n")) != NULL) {
        int value = atoi(token);
        if (value == current_value) {
            count++;
        } else {
            printf("%d:%d\n", current_value, count);
            current_value = value;
            count = 1;
        }
    }

    printf("%d:%d\n", current_value, count);

    return 0;
}