#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdbool.h>
#include <sys/stat.h>
#define BUFFER_SIZE 1024

//使わなかったのでエスケープ
// bool debug_mode = false;

//prototypes
bool is_valid_argument(int argc, char *argv[]);
bool file_copy(const char *source, const char *destination);
bool is_file_size_same(const char *file1, const char *file2);


bool is_valid_argument(int argc, char *argv[]){
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source> <destination> \n", argv[0]);
        return false;
    }

    // if (argc == 4 && strcmp(argv[3], "--debug") == 0) {
    //     debug_mode = true;
    // }

    if (access(argv[1], R_OK) == -1) {
        perror("Error accessing source file");
        return false;
    }

    // Check if destination file exists
    if (access(argv[2], F_OK) == -1) {
        return true;
    }else if (access(argv[2], W_OK) == -1)
    {
        fprintf(stderr, "Error: Destination file '%s' exists and is not writable.\n", argv[2]);
        return false;
    }
    
    return true;

}

bool file_copy(const char *source, const char *destination) {
    int src_fd = open(source, O_RDONLY);
    if (src_fd == -1) {
        perror("Error opening source file");
        return false;
    }

    int dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Error opening destination file");
        close(src_fd);
        return false;
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            perror("Error writing to destination file");
            close(src_fd);
            close(dest_fd);
            return false;
        }
    }

    close(src_fd);
    close(dest_fd);
    return true;
}

bool is_file_size_same(const char *file1, const char *file2) {
    struct stat st1, st2;
    if (stat(file1, &st1) == -1) {
        perror("Error getting file size");
        return false;
    }
    if (stat(file2, &st2) == -1) {
        perror("Error getting file size");
        return false;
    }
    return st1.st_size == st2.st_size;
}

int main (int argc, char *argv[]){

    if (!is_valid_argument(argc, argv)) {
        return 1;
    }
    if (!file_copy(argv[1], argv[2]))
    {
        fprintf(stderr, "Error: Failed to copy file '%s' to '%s'.\n", argv[1], argv[2]);
        return 1;
    }
    if (!is_file_size_same(argv[1], argv[2])) {
        fprintf(stderr, "Warning: The source and destination may not match.\n");
        return 1;
    }
    return 0;
}