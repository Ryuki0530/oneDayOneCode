#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define N 10000000
#define REPEAT 20

static uint32_t generate_value(uint32_t x)
{
    return x * 1664525u + 1013904223u;
}

static uint64_t heavy_calculation(const uint32_t *data, size_t size)
{
    uint64_t checksum = 0;

    for (int r = 0; r < REPEAT; r++) {
        for (size_t i = 0; i < size; i++) {
            uint32_t x = data[i];

            /*
             * 最適化の効果を観察しやすくするため、
             * 単純だが回数の多い整数演算を行う。
             */
            x = x ^ (x >> 16);
            x = x * 2654435761u;
            x = x ^ (x >> 13);

            checksum += x;
        }
    }

    return checksum;
}

int main(void)
{
    uint32_t *data = malloc(sizeof(uint32_t) * N);

    if (data == NULL) {
        fprintf(stderr, "メモリ確保に失敗しました\n");
        return 1;
    }

    uint32_t value = 123456789u;

    for (size_t i = 0; i < N; i++) {
        value = generate_value(value);
        data[i] = value;
    }

    clock_t start = clock();

    uint64_t checksum = heavy_calculation(data, N);

    clock_t end = clock();

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

    printf("checksum: %llu\n", (unsigned long long)checksum);
    printf("elapsed : %.3f sec\n", elapsed);

    free(data);

    return 0;
}