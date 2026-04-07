#include <bits/stdc++.h>
using namespace std;

int main() {
    int N;
    cin >> N;

    vector<long long> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i];

    long long sum = 0;
    for (int i = 0; i < N - 1; ++i) {
        sum += llabs(A[i] - A[i + 1]);
    }

    cout << sum << '\n';
    return 0;
}