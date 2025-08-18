#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define BUFFER_SIZE 1024

int openFile(const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
    }
    return fd;
}

char *load_file_one_buffer(int fd, int bufsize) {
    char *buffer = malloc(bufsize);
    if (buffer == NULL) {
        perror("Error allocating buffer");
        return NULL;
    }

    ssize_t bytesRead = read(fd, buffer, bufsize);
    if (bytesRead == -1) {
        perror("Error reading file");
        free(buffer);
        return NULL;
    }

    buffer[bytesRead] = '\0';  // Null-terminate the buffer
    return buffer;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fd = openFile(argv[1]);
    if (fd == -1) {
        return 1;
    }

    while (1)
    {
        char *buffer = load_file_one_buffer(fd, BUFFER_SIZE);
        if (buffer == NULL) {
            close(fd);
            return 1;
        }

        if (buffer[0] == '\0') {
            free(buffer);
            break;
        }

        write(STDOUT_FILENO, buffer, strlen(buffer));
        free(buffer);
    }

    close(fd);

    printf("\n");
    return 0;
}