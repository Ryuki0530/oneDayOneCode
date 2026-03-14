#include <stdio.h>
#include <stdlib.h>

typedef long long ll;

int cmp_ll(const void *a, const void *b) {
    ll va = *(ll*)a, vb = *(ll*)b;
    return (va > vb) - (va < vb);
}

int main() {
    int N;
    ll K;
    scanf("%d %lld", &N, &K);

    ll *a = (ll*)malloc(sizeof(ll) * N);
    for (int i = 0; i < N; i++) scanf("%lld", &a[i]);

    qsort(a, N, sizeof(ll), cmp_ll);

    ll ans = 0;
    int l = 0, r = N - 1;
    while (l < r) {
        ll sum = a[l] + a[r];
        if (sum < K) {
            l++;
        } else if (sum > K) {
            r--;
        } else {
            if (a[l] == a[r]) {
                ll cnt = r - l + 1;
                ans += cnt * (cnt - 1) / 2;
                break;
            }
            ll cnt_l = 1;
            while (l + cnt_l < r && a[l + cnt_l] == a[l]) cnt_l++;
            ll cnt_r = 1;
            while (r - cnt_r > l && a[r - cnt_r] == a[r]) cnt_r++;
            ans += cnt_l * cnt_r;
            l += cnt_l;
            r -= cnt_r;
        }
    }

    printf("%lld\n", ans);
    free(a);
    return 0;
}