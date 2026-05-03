#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    if (!(cin >> N)) return 0;
    vector<long long> P(N);
    for (int i = 0; i < N; ++i) cin >> P[i];

    vector<int> ans;
    ans.reserve(N);
    vector<pair<long long,int>> st;
    st.reserve(N);

    for (int i = 0; i < N; ++i) {
        long long price = P[i];
        int span = 1;
        while (!st.empty() && st.back().first <= price) {
            span += st.back().second;
            st.pop_back();
        }
        st.emplace_back(price, span);
        ans.push_back(span);
    }

    for (int i = 0; i < N; ++i) {
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << '\n';
    return 0;
}
