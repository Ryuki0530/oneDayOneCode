#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

long long get_file_size(const char *filepath) {
    struct stat st;
    if (stat(filepath, &st) == 0 && S_ISREG(st.st_mode)) {
        return (long long)st.st_size;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    const char *path;
    if (argc > 1) {
        path = argv[1];
    } else {
        printf("Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }

    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }
    struct dirent *entry;
    char full_path[1024];
    long long total_size = 0;
    
    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, ".", 1) == 0 || strncmp(entry->d_name, "..", 2) == 0) continue;
        
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
        long long size = get_file_size(full_path);
        if (size >= 0) {
            printf("%s Size: %lld bytes\n", entry->d_name, size);
            total_size += size;
        }
    }
    
    closedir(dir);
    printf("--------------------------------------\n");
    printf("Total: %lld bytes\n", total_size);
    printf("\n");
    return 0;
}
