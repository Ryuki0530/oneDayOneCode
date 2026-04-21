#include <iostream>
#include <string>

int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);

	std::string s;
	if (!(std::cin >> s)) return 0;

	std::string st;
	st.reserve(s.size());

	for (char c : s) {
		if (!st.empty() && st.back() == '(' && c == ')') {
			st.pop_back();
		} else {
			st.push_back(c);
		}
	}

	std::cout << st << '\n';
	return 0;
}

