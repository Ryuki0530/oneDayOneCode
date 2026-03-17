#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECORDS 10000
#define MAX_STR_LEN 64

typedef struct {
    char user[MAX_STR_LEN];
    char event[MAX_STR_LEN];
    int last_time;
} LastAccepted;

typedef struct {
    char name[MAX_STR_LEN];
    int count;
} CountEntry;

LastAccepted last_records[MAX_RECORDS];
CountEntry event_counts[MAX_RECORDS];
CountEntry user_counts[MAX_RECORDS];

static int find_last_record(LastAccepted records[], int n, const char *user, const char *event) {
    for (int i = 0; i < n; i++) {
        if (strcmp(records[i].user, user) == 0 && strcmp(records[i].event, event) == 0) {
            return i;
        }
    }
    return -1;
}

static int find_count_entry(CountEntry entries[], int n, const char *name) {
    for (int i = 0; i < n; i++) {
        if (strcmp(entries[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

static int cmp_count_entry(const void *a, const void *b) {
    const CountEntry *x = (const CountEntry *)a;
    const CountEntry *y = (const CountEntry *)b;
    return strcmp(x->name, y->name);
}

int main(void) {
    int W;

    if (scanf("%d", &W) != 1) {
        fprintf(stderr, "最初の整数Wを読めませんでした。\n");
        return 1;
    }

    int last_count = 0;
    int event_count_size = 0;
    int user_count_size = 0;
    int total = 0;

    int t;
    char user[MAX_STR_LEN];
    char event[MAX_STR_LEN];

    while (scanf("%d %63s %63s", &t, user, event) == 3) {
        int idx = find_last_record(last_records, last_count, user, event);
        int accept = 0;

        if (idx == -1) {
            accept = 1;
        } else {
            /* ちょうどW秒差は受理してよい */
            if (t - last_records[idx].last_time >= W) {
                accept = 1;
            }
        }

        if (accept) {
            total++;

            if (idx == -1) {
                if (last_count >= MAX_RECORDS) {
                    fprintf(stderr, "Too many records\n");
                    return 1;
                }
                strcpy(last_records[last_count].user, user);
                strcpy(last_records[last_count].event, event);
                last_records[last_count].last_time = t;
                last_count++;
            } else {
                last_records[idx].last_time = t;
            }

            int eidx = find_count_entry(event_counts, event_count_size, event);
            if (eidx == -1) {
                if (event_count_size >= MAX_RECORDS) {
                    fprintf(stderr, "Too many event types\n");
                    return 1;
                }
                strcpy(event_counts[event_count_size].name, event);
                event_counts[event_count_size].count = 1;
                event_count_size++;
            } else {
                event_counts[eidx].count++;
            }

            int uidx = find_count_entry(user_counts, user_count_size, user);
            if (uidx == -1) {
                if (user_count_size >= MAX_RECORDS) {
                    fprintf(stderr, "Too many users\n");
                    return 1;
                }
                strcpy(user_counts[user_count_size].name, user);
                user_counts[user_count_size].count = 1;
                user_count_size++;
            } else {
                user_counts[uidx].count++;
            }
        }
    }

    qsort(event_counts, event_count_size, sizeof(CountEntry), cmp_count_entry);
    qsort(user_counts, user_count_size, sizeof(CountEntry), cmp_count_entry);

    printf("TOTAL %d\n", total);

    printf("EVENTS\n");
    for (int i = 0; i < event_count_size; i++) {
        printf("%s %d\n", event_counts[i].name, event_counts[i].count);
    }

    printf("USERS\n");
    for (int i = 0; i < user_count_size; i++) {
        printf("%s %d\n", user_counts[i].name, user_counts[i].count);
    }

    return 0;
}