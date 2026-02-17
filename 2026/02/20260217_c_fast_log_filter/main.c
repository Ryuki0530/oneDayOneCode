#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_N 20000
#define MAX_Q 20000
#define MAX_LOG_LEN 201
#define MAX_PATTERN_LEN 51
#define NGRAM_SIZE 3
#define HASH_SIZE 10007

typedef struct {
    char *pattern;
    int *candidate_rows;
    int candidate_count;
} Query;

typedef struct {
    char ngram[NGRAM_SIZE + 1];
    int rows[MAX_N];
    int row_count;
} NGramEntry;

typedef struct {
    NGramEntry *entries;
    int count;
} NGramIndex;

static NGramIndex ngram_index;
static char logs[MAX_N][MAX_LOG_LEN];
static int N, Q;

unsigned int hash_ngram(const char *ngram) {
    unsigned int h = 0;
    for (int i = 0; i < NGRAM_SIZE && ngram[i]; i++) {
        h = h * 31 + (unsigned char)ngram[i];
    }
    return h % HASH_SIZE;
}

void build_ngram_index() {
    ngram_index.entries = malloc(sizeof(NGramEntry) * 100000);
    ngram_index.count = 0;
    
    for (int i = 0; i < N; i++) {
        int len = strlen(logs[i]);
        for (int j = 0; j <= len - NGRAM_SIZE; j++) {
            char ngram[NGRAM_SIZE + 1];
            strncpy(ngram, logs[i] + j, NGRAM_SIZE);
            ngram[NGRAM_SIZE] = '\0';
            
            NGramEntry *entry = NULL;
            for (int k = 0; k < ngram_index.count; k++) {
                if (strcmp(ngram_index.entries[k].ngram, ngram) == 0) {
                    entry = &ngram_index.entries[k];
                    break;
                }
            }
            
            if (!entry) {
                entry = &ngram_index.entries[ngram_index.count++];
                strcpy(entry->ngram, ngram);
                entry->row_count = 0;
            }
            
            bool found = false;
            for (int k = 0; k < entry->row_count; k++) {
                if (entry->rows[k] == i) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                entry->rows[entry->row_count++] = i;
            }
        }
    }
}

int filter_candidates(int *candidates, const char *pattern) {
    int pattern_len = strlen(pattern);
    
    if (pattern_len < NGRAM_SIZE) {
        int count = 0;
        for (int i = 0; i < N; i++) {
            candidates[count++] = i;
        }
        return count;
    }
    
    int result_rows[MAX_N];
    int result_count = -1;
    
    for (int start = 0; start <= pattern_len - NGRAM_SIZE; start++) {
        char ngram[NGRAM_SIZE + 1];
        strncpy(ngram, pattern + start, NGRAM_SIZE);
        ngram[NGRAM_SIZE] = '\0';
        
        NGramEntry *entry = NULL;
        for (int k = 0; k < ngram_index.count; k++) {
            if (strcmp(ngram_index.entries[k].ngram, ngram) == 0) {
                entry = &ngram_index.entries[k];
                break;
            }
        }
        
        if (!entry) {
            return 0;
        }
        
        if (result_count == -1) {
            memcpy(result_rows, entry->rows, entry->row_count * sizeof(int));
            result_count = entry->row_count;
        } else {
            int new_count = 0;
            for (int i = 0; i < result_count; i++) {
                for (int j = 0; j < entry->row_count; j++) {
                    if (result_rows[i] == entry->rows[j]) {
                        candidates[new_count++] = result_rows[i];
                        break;
                    }
                }
            }
            result_count = new_count;
            memcpy(result_rows, candidates, result_count * sizeof(int));
        }
    }
    
    return result_count;
}

int main() {
    scanf("%d %d", &N, &Q);
    
    for (int i = 0; i < N; i++) {
        scanf("%s", logs[i]);
    }
    
    build_ngram_index();
    
    for (int i = 0; i < Q; i++) {
        char pattern[MAX_PATTERN_LEN];
        scanf("%s", pattern);
        
        int candidates[MAX_N];
        int cand_count = filter_candidates(candidates, pattern);
        
        int count = 0;
        for (int j = 0; j < cand_count; j++) {
            if (strstr(logs[candidates[j]], pattern)) {
                count++;
            }
        }
        
        printf("%d\n", count);
    }
    
    free(ngram_index.entries);
    return 0;
}