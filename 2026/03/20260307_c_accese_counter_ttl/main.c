#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long long time;
    char user[256];
} AccessRecord;

int main() {
    int Q;
    long long W;
    scanf("%d %lld", &Q, &W);
    
    AccessRecord *records = (AccessRecord *)malloc(sizeof(AccessRecord) * Q);
    int record_count = 0;
    
    for (int i = 0; i < Q; i++) {
        char command[10];
        scanf("%s", command);
        
        if (strcmp(command, "ADD") == 0) {
            long long t;
            char user[256];
            scanf("%lld %s", &t, user);
            
            records[record_count].time = t;
            strcpy(records[record_count].user, user);
            record_count++;
        } 
        else if (strcmp(command, "COUNT") == 0) {
            long long t;
            scanf("%lld", &t);
            
            int count = 0;
            for (int j = 0; j < record_count; j++) {
                if (records[j].time > t - W) {
                    count++;
                }
            }
            printf("%d\n", count);
        }
    }
    
    free(records);
    return 0;
}