#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <sys/wait.h>

#define INPUT_MAX_SIZE 1024

int parent_process(int fd[2]);
int child_process(int fd[2]);

int main(){
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1){
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }else if(pid ==0){
        if (child_process(fd)){
            printf("[ERROR] Chiled process failed .");
            exit(EXIT_FAILURE);
        }
    }else{
        if (parent_process(fd)){
            printf("[ERROR] Parent process failed .");
            exit(EXIT_FAILURE);
        }
        wait(NULL); // Wait for child process to finish
    }
}



int parent_process(int fd[2]){
    close(fd[0]); //読み取り口閉鎖
    char input[INPUT_MAX_SIZE];
    printf("Plaease enter text :");
    if (fgets(input, INPUT_MAX_SIZE,stdin) == NULL){
        perror("fgets");
        close(fd[1]);
        return 1;
    }

    input[strcspn(input, "\n") ] = '\0';

    if (write(fd[1], input, strlen(input) + 1) == -1){
        perror("write");
        return 1;
    }
    close(fd[1]);

    return 0;
}


int child_process(int fd[2]){
    close(fd[1]); //書き込み側閉鎖
    char buffer[INPUT_MAX_SIZE];
    ssize_t bytes_read = read(fd[0], buffer, sizeof(buffer));

    if (bytes_read == -1){
        perror("read");
        close(fd[0]);
        return 1;
    }

    for (int i = 0; buffer[i]; i++) {
        buffer[i] = toupper((unsigned char)buffer[i]);
    }

    printf("Uppercase string: %s\n", buffer);
    close(fd[0]);

    return 0;
}
