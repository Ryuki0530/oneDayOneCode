#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long long time;
    int idx;
} TimePoint;

int compare_time(const void *a, const void *b) {
    TimePoint *ta = (TimePoint *)a;
    TimePoint *tb = (TimePoint *)b;
    if (ta->time != tb->time)
        return ta->time < tb->time ? -1 : 1;
    return 0;
}

int main() {
    int n;
    scanf("%d", &n);
    
    long long *s = (long long *)malloc(n * sizeof(long long));
    long long *e = (long long *)malloc(n * sizeof(long long));
    
    for (int i = 0; i < n; i++) {
        scanf("%lld %lld", &s[i], &e[i]);
    }
    
    TimePoint *times = (TimePoint *)malloc(2 * n * sizeof(TimePoint));
    for (int i = 0; i < n; i++) {
        times[2 * i].time = s[i];
        times[2 * i].idx = i;
        times[2 * i + 1].time = e[i];
        times[2 * i + 1].idx = i;
    }
    
    qsort(times, 2 * n, sizeof(TimePoint), compare_time);
    
    long long *t = (long long *)malloc(2 * n * sizeof(long long));
    int m = 0;
    for (int i = 0; i < 2 * n; i++) {
        if (i == 0 || times[i].time != times[i - 1].time) {
            t[m++] = times[i].time;
        }
    }
    
    int *diff = (int *)calloc(m, sizeof(int));
    for (int i = 0; i < n; i++) {
        int si = 0, ei = 0;
        for (int j = 0; j < m; j++) {
            if (t[j] == s[i]) si = j;
            if (t[j] == e[i]) ei = j;
        }
        diff[si] += 1;
        diff[ei] -= 1;
    }
    
    int maxK = 0;
    long long totalMinutes = 0;
    int cur = 0;
    
    for (int i = 0; i < m - 1; i++) {
        cur += diff[i];
        if (cur > maxK) {
            maxK = cur;
            totalMinutes = t[i + 1] - t[i];
        } else if (cur == maxK) {
            totalMinutes += t[i + 1] - t[i];
        }
    }
    
    printf("%d\n%lld\n", maxK, totalMinutes);
    
    free(s);
    free(e);
    free(times);
    free(t);
    free(diff);
    
    return 0;
}