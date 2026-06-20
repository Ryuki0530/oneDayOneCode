#include <stdio.h>

int detect_timeout(const int timestamps[], int size, int timeout_ms) {
    int count = 0;
    for (int i = 1; i < size; ++i) {
        int diff = timestamps[i] - timestamps[i - 1];
        if (diff > timeout_ms) {
            puts("TIMEOUT DETECTED");
            ++count;
        }
    }
    return count;
}

int main(void) {
    int timestamps[] = {
        0, 100, 200, 300, 400,
        500, 600, 700, 1200, 1300
    };

    int size = sizeof timestamps / sizeof timestamps[0];
    int timeout_ms = 300;

    int timeout_count = detect_timeout(timestamps, size, timeout_ms);
    printf("timeout_count = %d\n", timeout_count);

    return 0;
}
