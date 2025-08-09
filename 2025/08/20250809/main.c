#include <unistd.h>
#include <string.h>
#include <errno.h>

#define STDIN_DESCRIPTOR 0
#define STDOUT_DESCRIPTOR 1
#define STDERR_DESCRIPTOR 2
#define BUFFER_SIZE 1024

int main() {
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(STDIN_DESCRIPTOR, buffer, BUFFER_SIZE)) > 0) {
        if (write(STDOUT_DESCRIPTOR, buffer, bytesRead) < 0) {
            const char *errMsg = strerror(errno);
            write(STDERR_DESCRIPTOR, errMsg, strlen(errMsg));
            write(STDERR_DESCRIPTOR, "\n", 1);
            return 1;
        }
    }

    if (bytesRead < 0) {
        const char *errMsg = strerror(errno);
        write(STDERR_DESCRIPTOR, errMsg, strlen(errMsg));
        write(STDERR_DESCRIPTOR, "\n", 1);
        return 1;
    }
    return 0;
}