#include <stdio.h>
#include <string.h>

#define MAX_RECORDS 1000
#define MAX_LEN 64

typedef struct {
    char user[MAX_LEN];
    char event[MAX_LEN];
    int last_time;
} LastAccepted;

typedef struct {
    char event[MAX_LEN];
    int count;
} EventCount;

int find_last_index(LastAccepted arr[], int n, const char *user, const char *event) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].user, user) == 0 && strcmp(arr[i].event, event) == 0) {
            return i;
        }
    }
    return -1;
}

int find_event_index(EventCount arr[], int n, const char *event) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].event, event) == 0) {
            return i;
        }
    }
    return -1;
}

void sort_events(EventCount arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (strcmp(arr[i].event, arr[j].event) > 0) {
                EventCount tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }
}

int main(void) {
    int W;
    LastAccepted lastAccepted[MAX_RECORDS];
    EventCount eventCounts[MAX_RECORDS];
    int lastCount = 0;
    int eventCountSize = 0;
    int totalAccepted = 0;

    if (scanf("%d", &W) != 1) {
        return 0;
    }

    int timestamp;
    char user[MAX_LEN];
    char event[MAX_LEN];

    while (scanf("%d %63s %63s", &timestamp, user, event) == 3) {
        int idx = find_last_index(lastAccepted, lastCount, user, event);
        int accepted = 0;

        if (idx == -1) {
            strcpy(lastAccepted[lastCount].user, user);
            strcpy(lastAccepted[lastCount].event, event);
            lastAccepted[lastCount].last_time = timestamp;
            lastCount++;
            accepted = 1;
        } else {
            if (timestamp - lastAccepted[idx].last_time > W) {
                lastAccepted[idx].last_time = timestamp;
                accepted = 1;
            }
        }

        if (accepted) {
            totalAccepted++;

            int eidx = find_event_index(eventCounts, eventCountSize, event);
            if (eidx == -1) {
                strcpy(eventCounts[eventCountSize].event, event);
                eventCounts[eventCountSize].count = 1;
                eventCountSize++;
            } else {
                eventCounts[eidx].count++;
            }
        }
    }

    sort_events(eventCounts, eventCountSize);

    printf("TOTAL %d\n", totalAccepted);
    for (int i = 0; i < eventCountSize; i++) {
        printf("EVENT %s %d\n", eventCounts[i].event, eventCounts[i].count);
    }

    return 0;
}