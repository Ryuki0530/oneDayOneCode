#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS 1000
#define MAX_USER_ID 21
#define MAX_LOGS 1000

typedef struct {
    char user_id[MAX_USER_ID];
    int count;
    int last_time;
} UserData;

int compare_users(const void *a, const void *b) {
    return strcmp(((UserData *)a)->user_id, ((UserData *)b)->user_id);
}

int main() {
    int n;
    scanf("%d", &n);
    
    UserData users[MAX_USERS];
    int user_count = 0;
    
    for (int i = 0; i < n; i++) {
        int t;
        char user_id[MAX_USER_ID];
        scanf("%d %s", &t, user_id);
        
        int user_idx = -1;
        for (int j = 0; j < user_count; j++) {
            if (strcmp(users[j].user_id, user_id) == 0) {
                user_idx = j;
                break;
            }
        }
        
        if (user_idx == -1) {
            strcpy(users[user_count].user_id, user_id);
            users[user_count].count = 1;
            users[user_count].last_time = t;
            user_count++;
        } else {
            if (t - users[user_idx].last_time <= 5) {
            } else {
                users[user_idx].count++;
            }
            users[user_idx].last_time = t;
        }
    }
    
    qsort(users, user_count, sizeof(UserData), compare_users);
    
    for (int i = 0; i < user_count; i++) {
        printf("%s %d\n", users[i].user_id, users[i].count);
    }
    
    return 0;
}