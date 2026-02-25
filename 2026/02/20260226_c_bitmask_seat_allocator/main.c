#include <stdio.h>
#include <stdint.h>
#include <string.h>

int main() {
    uint64_t seats = 0;
    int Q;
    scanf("%d", &Q);
    
    while (Q--) {
        char cmd[10];
        scanf("%s", cmd);
        
        if (strcmp(cmd, "RESERVE") == 0) {
            int k;
            scanf("%d", &k);
            
            int found = -1;
            for (int s = 0; s <= 64 - k; s++) {
                uint64_t mask = (k == 64) ? ~0ULL : ((1ULL << k) - 1) << s;
                if ((seats & mask) == 0) {
                    seats |= mask;
                    found = s;
                    break;
                }
            }
            printf("%d\n", found);
            
        } else if (strcmp(cmd, "CANCEL") == 0) {
            int s, k;
            scanf("%d %d", &s, &k);
            
            for (int i = s; i < s + k && i < 64; i++) {
                seats &= ~(1ULL << i);
            }
            
        } else if (strcmp(cmd, "STATUS") == 0) {
            printf("%d\n", __builtin_popcountll(seats));
            
        } else if (strcmp(cmd, "PRINT") == 0) {
            for (int i = 63; i >= 0; i--) {
                printf("%d", (seats >> i) & 1);
            }
            printf("\n");
        }
    }
    
    return 0;
}