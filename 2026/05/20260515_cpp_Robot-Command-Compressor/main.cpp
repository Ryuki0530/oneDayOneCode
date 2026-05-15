#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string S;
    if (!(cin >> S)) return 0;

    vector<pair<char, int>> compressed;
    long long x = 0, y = 0;

    auto apply = [&](char c) {
        if (c == 'U') ++y;
        else if (c == 'D') --y;
        else if (c == 'R') ++x;
        else if (c == 'L') --x;
    };

    char prev = S[0];
    int cnt = 1;
    apply(prev);

    for (size_t i = 1; i < S.size(); ++i) {
        char c = S[i];
        apply(c);
        if (c == prev) {
            ++cnt;
        } else {
            compressed.push_back({prev, cnt});
            prev = c;
            cnt = 1;
        }
    }
    compressed.push_back({prev, cnt});

    cout << compressed.size() << '\n';
    for (const auto& [c, n] : compressed) {
        cout << c << ' ' << n << '\n';
    }
    cout << x << ' ' << y << '\n';
    cout << "DIST " << (llabs(x) + llabs(y)) << '\n';
    return 0;
}