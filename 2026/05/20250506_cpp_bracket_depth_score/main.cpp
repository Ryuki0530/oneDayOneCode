#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string S;
    if(!(cin >> S)) return 0;

    int depth = 0, maxd = 0;
    for(char c: S) {
        if(c == '(') {
            ++depth;
            if(depth > maxd) maxd = depth;
        } else if(c == ')') {
            --depth;
            if(depth < 0) {
                cout << "INVALID\n";
                return 0;
            }
        } else {
            // invalid character (per problem won't happen)
            cout << "INVALID\n";
            return 0;
        }
    }

    if(depth != 0) cout << "INVALID\n";
    else cout << maxd << '\n';

    return 0;
}
