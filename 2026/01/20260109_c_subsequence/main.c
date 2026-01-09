#include <stdio.h>
#include <string.h>

int main() {
    char S[200005], T[200005];
    
    fgets(S, sizeof(S), stdin);
    fgets(T, sizeof(T), stdin);
    
    S[strcspn(S, "\n")] = '\0';
    T[strcspn(T, "\n")] = '\0';
    
    int s_len = strlen(S);
    int t_len = strlen(T);
    
    int s_idx = 0;
    int t_idx = 0;
    
    while (t_idx < t_len && s_idx < s_len) {
        if (S[s_idx] == T[t_idx]) {
            s_idx++;
        }
        t_idx++;
    }
    
    if (s_idx == s_len) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
    
    return 0;
}