#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N, Q;
	if (!(cin >> N >> Q)) return 0;

	vector<int> mn(N), mx(N), ang(N);
	for (int i = 0; i < N; ++i) {
		cin >> mn[i] >> mx[i] >> ang[i];
	}

	int executed = 0, rejected = 0;
	for (int i = 0; i < Q; ++i) {
		int joint, delta;
		cin >> joint >> delta;
		--joint;
		int next = ang[joint] + delta;
		if (mn[joint] <= next && next <= mx[joint]) {
			ang[joint] = next;
			++executed;
		} else {
			++rejected;
		}
	}

	cout << executed << ' ' << rejected << '\n';
	for (int i = 0; i < N; ++i) {
		if (i) cout << ' ';
		cout << ang[i];
	}
	cout << '\n';
	return 0;
}
