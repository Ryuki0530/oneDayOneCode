#include <stdio.h>
#include <stdint.h>

#define BLOCK_SIZE 32
#define BLOCK_COUNT 8

typedef struct {
    unsigned char memory[BLOCK_COUNT][BLOCK_SIZE];
    int free_queue[BLOCK_COUNT];
    int head;
    int tail;
    int free_count;
    unsigned char allocated[BLOCK_COUNT];
} MemoryPool;

void pool_init(MemoryPool *pool)
{
    int i;

    for (i = 0; i < BLOCK_COUNT; ++i) {
        pool->allocated[i] = 0;
        pool->free_queue[i] = i;
    }

    pool->head = 0;
    pool->tail = 0;
    pool->free_count = BLOCK_COUNT;
}

void *pool_alloc(MemoryPool *pool)
{
    int block;

    if (pool->free_count == 0) {
        return NULL;
    }

    block = pool->free_queue[pool->head];
    pool->head = (pool->head + 1) % BLOCK_COUNT;
    pool->allocated[block] = 1;
    --pool->free_count;

    return pool->memory[block];
}

int pool_free(MemoryPool *pool, void *ptr)
{
    uintptr_t pool_start;
    uintptr_t target;
    uintptr_t offset;
    int block;

    if (ptr == NULL) {
        return -1;
    }

    pool_start = (uintptr_t)(void *)pool->memory;
    target = (uintptr_t)ptr;

    if (target < pool_start) {
        return -1;
    }

    offset = target - pool_start;
    if (offset >= (uintptr_t)(BLOCK_COUNT * BLOCK_SIZE)) {
        return -1;
    }

    if (offset % BLOCK_SIZE != 0) {
        return -1;
    }

    block = (int)(offset / BLOCK_SIZE);
    if (pool->allocated[block] == 0) {
        return -1;
    }

    pool->allocated[block] = 0;
    pool->free_queue[pool->tail] = block;
    pool->tail = (pool->tail + 1) % BLOCK_COUNT;
    ++pool->free_count;

    return 0;
}

int pool_available(const MemoryPool *pool)
{
    return pool->free_count;
}

int main(void)
{
    MemoryPool pool;
    void *blocks[BLOCK_COUNT];
    void *reallocated;
    int i;

    pool_init(&pool);
    printf("Available: %d\n", pool_available(&pool));

    for (i = 0; i < BLOCK_COUNT; ++i) {
        blocks[i] = pool_alloc(&pool);
    }

    printf("Allocated: %d blocks\n", BLOCK_COUNT);
    printf("Available: %d\n", pool_available(&pool));

    if (pool_alloc(&pool) == NULL) {
        printf("Allocation failed\n");
    }

    if (pool_free(&pool, blocks[0]) == 0) {
        printf("Available after free: %d\n", pool_available(&pool));
    }

    reallocated = pool_alloc(&pool);
    if (reallocated != NULL) {
        printf("Reallocated successfully\n");
    }

    printf("Available: %d\n", pool_available(&pool));

    if (pool_free(&pool, blocks[0]) == -1) {
        printf("Double free detected\n");
    }

    return 0;
}
