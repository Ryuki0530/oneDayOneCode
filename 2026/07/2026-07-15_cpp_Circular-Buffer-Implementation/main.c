#include <stdio.h>
#define BUFFER_SIZE 5

typedef struct {
    int data[BUFFER_SIZE];
    int head;
    int tail;
    int size;
} CircularBuffer;

void init_buffer(CircularBuffer *buf) {
    buf->head = 0;
    buf->tail = 0;
    buf->size = 0;
}

int push(CircularBuffer *buf, int value) {
    if (buf->size == BUFFER_SIZE) return -1;
    buf->data[buf->tail] = value;
    buf->tail = (buf->tail + 1) % BUFFER_SIZE;
    buf->size++;
    return 0;
}

int pop(CircularBuffer *buf, int *value) {
    if (buf->size == 0) return -1;
    *value = buf->data[buf->head];
    buf->head = (buf->head + 1) % BUFFER_SIZE;
    buf->size--;
    return 0;
}

void print_buffer(const CircularBuffer *buf) {
    printf("[");
    for (int i = 0; i < buf->size; ++i) {
        int idx = (buf->head + i) % BUFFER_SIZE;
        printf("%s%d", (i? "][" : ""), buf->data[idx]);
    }
    if (buf->size > 0) printf("]");
    printf("\n");
}

int main(void) {
    CircularBuffer buf;
    init_buffer(&buf);

    push(&buf, 10); printf("push 10\n");
    push(&buf, 20); printf("push 20\n");
    push(&buf, 30); printf("push 30\n");

    int v;
    if (pop(&buf, &v) == 0) printf("pop -> %d\n", v);

    push(&buf, 40); printf("push 40\n");
    push(&buf, 50); printf("push 50\n");
    push(&buf, 60); printf("push 60\n");

    while (pop(&buf, &v) == 0) printf("pop -> %d\n", v);

    return 0;
}
