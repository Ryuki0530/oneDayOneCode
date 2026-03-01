#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define WORD_BITS 64
#define MAX_WORDS 160000

typedef uint64_t word_t;

word_t bits[MAX_WORDS];
int N, Q;
int cursor = 0;

int alloc_id(void) {
    int start_w = cursor / WORD_BITS;
    int start_b = cursor % WORD_BITS;
    
    for (int w = start_w; w < MAX_WORDS; w++) {
        word_t free_bits = ~bits[w];
        if (free_bits == 0) continue;
        
        int b = (w == start_w) ? start_b : 0;
        for (int i = b; i < WORD_BITS; i++) {
            if ((free_bits >> i) & 1) {
                int id = w * WORD_BITS + i;
                if (id >= N) return -1;
                bits[w] |= (1ULL << i);
                cursor = id;
                return id;
            }
        }
    }
    return -1;
}

void free_id(int x) {
    if (x < 0 || x >= N) return;
    int w = x / WORD_BITS;
    int b = x % WORD_BITS;
    bits[w] &= ~(1ULL << b);
    if (x < cursor) cursor = x;
}

int test_id(int x) {
    if (x < 0 || x >= N) return 0;
    int w = x / WORD_BITS;
    int b = x % WORD_BITS;
    return (bits[w] >> b) & 1;
}

int main(void) {
    scanf("%d %d", &N, &Q);
    memset(bits, 0, sizeof(bits));
    
    for (int i = 0; i < Q; i++) {
        char cmd[10];
        scanf("%s", cmd);
        
        if (cmd[0] == 'A') {
            printf("%d\n", alloc_id());
        } else if (cmd[0] == 'F') {
            int x;
            scanf("%d", &x);
            free_id(x);
        } else if (cmd[0] == 'T') {
            int x;
            scanf("%d", &x);
            printf("%d\n", test_id(x));
        }
    }
    
    return 0;
}