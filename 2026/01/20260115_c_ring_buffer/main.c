#include <stdio.h>
#include <stdlib.h>

int main() {
    int K;
    scanf("%d", &K);
    
    int buf[K];
    long long sum = 0;
    
    int i = 0;
    int A;
    while (scanf("%d", &A) == 1) {
        int pos = i % K;
        
        if (i < K) {
            sum += A;
        } else {
            sum += A - buf[pos];
        }
        
        buf[pos] = A;
        
        int window_size = (i < K) ? (i + 1) : K;
        double avg = sum / (double)window_size;
        printf("%.1f\n", avg);
        
        i++;
    }
    
    return 0;
}