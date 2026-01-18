#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE 200005
#define HASH_SIZE 100007

typedef struct {
    int code;
    int freq;
} FreqEntry;

typedef struct {
    int code;
    int timestamp;
} LogEntry;

typedef struct {
    LogEntry data[MAX_QUEUE];
    int front, rear, size;
} Queue;

typedef struct {
    int code;
    int freq;
    int active;
} HashEntry;

HashEntry hash_table[HASH_SIZE];

int hash_fn(int code) {
    return ((long long)code * 2654435761LL) % HASH_SIZE;
}

void hash_insert(int code, int freq) {
    int idx = hash_fn(code);
    while (hash_table[idx].active && hash_table[idx].code != code) {
        idx = (idx + 1) % HASH_SIZE;
    }
    hash_table[idx].code = code;
    hash_table[idx].freq = freq;
    hash_table[idx].active = 1;
}

int hash_get(int code) {
    int idx = hash_fn(code);
    while (hash_table[idx].active) {
        if (hash_table[idx].code == code) {
            return hash_table[idx].freq;
        }
        idx = (idx + 1) % HASH_SIZE;
    }
    return 0;
}

void hash_clear() {
    memset(hash_table, 0, sizeof(hash_table));
}

Queue* queue_create() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    return q;
}

void queue_push(Queue *q, int code, int timestamp) {
    q->rear = (q->rear + 1) % MAX_QUEUE;
    q->data[q->rear].code = code;
    q->data[q->rear].timestamp = timestamp;
    q->size++;
}

LogEntry queue_pop(Queue *q) {
    LogEntry entry = q->data[q->front];
    q->front = (q->front + 1) % MAX_QUEUE;
    q->size--;
    return entry;
}

int main(void) {
    int W;
    if (scanf("%d", &W) != 1) return 0;
    
    Queue *q = queue_create();
    int t, code;
    
    while (scanf("%d %d", &t, &code) == 2) {
        // Remove logs outside window
        while (q->size > 0 && q->data[q->front].timestamp < t - W + 1) {
            LogEntry old = queue_pop(q);
            int old_freq = hash_get(old.code);
            if (old_freq > 1) {
                hash_insert(old.code, old_freq - 1);
            }
        }
        
        // Add new log
        queue_push(q, code, t);
        int new_freq = hash_get(code);
        hash_insert(code, new_freq + 1);
        
        // Find mode (max freq, then min code)
        int max_freq = 0;
        int mode_code = INT_MAX;
        
        for (int i = 0; i < HASH_SIZE; i++) {
            if (hash_table[i].active && hash_table[i].freq > 0) {
                if (hash_table[i].freq > max_freq || 
                    (hash_table[i].freq == max_freq && hash_table[i].code < mode_code)) {
                    max_freq = hash_table[i].freq;
                    mode_code = hash_table[i].code;
                }
            }
        }
        
        printf("%d\n", mode_code);
    }
    
    free(q);
    return 0;
}