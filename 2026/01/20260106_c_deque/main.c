#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int front, rear, capacity;
} Deque;

Deque* createDeque(int capacity) {
    Deque *dq = (Deque*)malloc(sizeof(Deque));
    dq->data = (int*)malloc(sizeof(int) * capacity);
    dq->front = 0;
    dq->rear = -1;
    dq->capacity = capacity;
    return dq;
}

int isEmpty(Deque *dq) {
    return dq->rear < dq->front;
}

void pushBack(Deque *dq, int val) {
    dq->data[++dq->rear] = val;
}

int popFront(Deque *dq) {
    return dq->data[dq->front++];
}

int getFront(Deque *dq) {
    return dq->data[dq->front];
}

int main() {
    int N, K;
    scanf("%d %d", &N, &K);
    
    int *A = (int*)malloc(sizeof(int) * N);
    for (int i = 0; i < N; i++) {
        scanf("%d", &A[i]);
    }
    
    Deque *dq = createDeque(N);

    for (int i = 0; i < K; i++) {
        while (!isEmpty(dq) && A[dq->rear] <= A[i]) {
            dq->rear--;
            dq->front = (dq->front > dq->rear) ? 0 : dq->front;
        }
        pushBack(dq, i);
    }
    
    printf("%d", A[getFront(dq)]);
    
    for (int i = K; i < N; i++) {
        if (dq->front <= dq->rear && dq->data[dq->front] <= i - K) {
            popFront(dq);
        }
        while (!isEmpty(dq) && A[dq->rear] <= A[i]) {
            dq->rear--;
            dq->front = (dq->front > dq->rear) ? 0 : dq->front;
        }
        pushBack(dq, i);
        printf(" %d", A[getFront(dq)]);
    }
    printf("\n");
    
    free(A);
    free(dq->data);
    free(dq);
    
    return 0;
}