#include <bits/stdc++.h>
using namespace std;

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int N;
	cin >> N;

	map<string, string> last;
	for (int i = 0; i < N; ++i) {
		string command, filename;
		cin >> command >> filename;
		last[filename] = command;
	}

	bool found = false;
	for (const auto& [filename, command] : last) {
		if (command == "OPEN" || command == "SAVE") {
			cout << filename << '\n';
			found = true;
		}
	}

	if (!found) cout << "NONE\n";
	return 0;
}
