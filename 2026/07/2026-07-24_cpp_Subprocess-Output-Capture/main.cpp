#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

int main(int argc, char* argv[]) {
    int pipefd[2];
    pid_t pid;
    char buffer[4096];
    ssize_t bytes_read;

    if (pipe(pipefd) == -1) {
        std::cerr << "pipe() failed" << std::endl;
        return 1;
    }

    pid = fork();
    if (pid == -1) {
        std::cerr << "fork() failed" << std::endl;
        return 1;
    }

    if (pid == 0) {
        close(pipefd[0]);
        
        if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
            std::cerr << "dup2() failed" << std::endl;
            return 1;
        }
        
        close(pipefd[1]);
        const char* cmd;
        char** args;
        
        if (argc > 1) {
            cmd = argv[1];
            args = &argv[1];
        } else {
            cmd = "uname";
            static const char* default_args[] = {"uname", "-a", nullptr};
            args = const_cast<char**>(default_args);
        }

        if (execvp(cmd, args) == -1) {
            std::cerr << "execvp() failed" << std::endl;
            return 1;
        }
        
        return 0;
    } else {
        close(pipefd[1]);

        std::cout << "Child output:" << std::endl;
        while ((bytes_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            std::cout.write(buffer, bytes_read);
        }

        if (bytes_read == -1) {
            std::cerr << "read() failed" << std::endl;
            close(pipefd[0]);
            return 1;
        }

        close(pipefd[0]);

        int status;
        if (waitpid(pid, &status, 0) == -1) {
            std::cerr << "waitpid() failed" << std::endl;
            return 1;
        }

        return 0;
    }
}