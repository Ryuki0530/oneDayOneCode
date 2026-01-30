#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int size;
    int capacity;
} MinHeap;

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void heapify_up(MinHeap *heap, int idx) {
    while (idx > 0) {
        int parent = (idx - 1) / 2;
        if (heap->data[idx] < heap->data[parent]) {
            swap(&heap->data[idx], &heap->data[parent]);
            idx = parent;
        } else {
            break;
        }
    }
}

void heapify_down(MinHeap *heap, int idx) {
    while (1) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;
        
        if (left < heap->size && heap->data[left] < heap->data[smallest])
            smallest = left;
        if (right < heap->size && heap->data[right] < heap->data[smallest])
            smallest = right;
        
        if (smallest != idx) {
            swap(&heap->data[idx], &heap->data[smallest]);
            idx = smallest;
        } else {
            break;
        }
    }
}

void push(MinHeap *heap, int val, int k) {
    if (heap->size < k) {
        heap->data[heap->size] = val;
        heapify_up(heap, heap->size);
        heap->size++;
    } else if (val > heap->data[0]) {
        heap->data[0] = val;
        heapify_down(heap, 0);
    }
}

int main() {
    int n, k;
    scanf("%d %d", &n, &k);
    
    MinHeap heap;
    heap.capacity = k;
    heap.data = (int *)malloc(k * sizeof(int));
    heap.size = 0;
    
    for (int i = 0; i < n; i++) {
        int val;
        scanf("%d", &val);
        push(&heap, val, k);
    }
    
    int *result = (int *)malloc(heap.size * sizeof(int));
    for (int i = 0; i < heap.size; i++) {
        result[i] = heap.data[i];
    }
    
    for (int i = 0; i < heap.size - 1; i++) {
        for (int j = i + 1; j < heap.size; j++) {
            if (result[i] < result[j])
                swap(&result[i], &result[j]);
        }
    }
    
    for (int i = 0; i < heap.size; i++) {
        if (i > 0) printf(" ");
        printf("%d", result[i]);
    }
    printf("\n");
    
    free(heap.data);
    free(result);
    return 0;
}