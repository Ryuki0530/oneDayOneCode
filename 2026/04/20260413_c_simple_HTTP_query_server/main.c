#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define PORT 8080
#define BUFFER_SIZE 4096

void send_response(int client_fd, const char *body) {
    char response[BUFFER_SIZE];
    snprintf(response, sizeof(response),
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: text/plain\r\n"
             "Content-Length: %lu\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s", strlen(body), body);
    send(client_fd, response, strlen(response), 0);
}

void parse_query(const char *query, char *response) {
    char query_copy[BUFFER_SIZE];
    strncpy(query_copy, query, sizeof(query_copy) - 1);
    query_copy[sizeof(query_copy) - 1] = '\0';

    // Check for name parameter
    char *name_ptr = strstr(query_copy, "name=");
    if (name_ptr) {
        name_ptr += 5;
        char name[256] = {0};
        sscanf(name_ptr, "%255[^&]", name);
        snprintf(response, BUFFER_SIZE, "Hello, %s!", name);
        return;
    }

    // Check for a and b parameters
    char *a_ptr = strstr(query_copy, "a=");
    char *b_ptr = strstr(query_copy, "b=");
    if (a_ptr && b_ptr) {
        int a = 0, b = 0;
        sscanf(a_ptr + 2, "%d", &a);
        sscanf(b_ptr + 2, "%d", &b);
        snprintf(response, BUFFER_SIZE, "sum=%d", a + b);
        return;
    }

    // Check for mode=upper and text parameter
    char *mode_ptr = strstr(query_copy, "mode=upper");
    char *text_ptr = strstr(query_copy, "text=");
    if (mode_ptr && text_ptr) {
        text_ptr += 5;
        char text[256] = {0};
        sscanf(text_ptr, "%255[^&]", text);
        for (int i = 0; text[i]; i++) {
            text[i] = toupper((unsigned char)text[i]);
        }
        snprintf(response, BUFFER_SIZE, "%s", text);
        return;
    }

    // Default: bad request
    snprintf(response, BUFFER_SIZE, "bad request");
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 1) < 0) {
        perror("listen");
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        struct sockaddr_in client_addr = {0};
        socklen_t client_addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        char buffer[BUFFER_SIZE] = {0};
        recv(client_fd, buffer, sizeof(buffer) - 1, 0);

        // Parse first line: "GET /?query HTTP/1.1"
        char method[16] = {0}, path[BUFFER_SIZE] = {0}, query[BUFFER_SIZE] = {0};
        sscanf(buffer, "%15s %4095s HTTP", method, path);

        // Extract query string from path
        char *query_start = strchr(path, '?');
        if (query_start) {
            strncpy(query, query_start + 1, sizeof(query) - 1);
            query[sizeof(query) - 1] = '\0';
        }

        char response[BUFFER_SIZE] = {0};
        parse_query(query, response);
        send_response(client_fd, response);

        close(client_fd);
    }

    close(server_fd);
    return 0;
}