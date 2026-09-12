#include <algorithm>
#include <iostream>
#include <vector>

int main() {
	std::vector<int> values{2, 4, 7, 10, 8, 12, 15};

	const auto it = std::adjacent_find(
		values.begin(), values.end(), [](int left, int right) {
			return left > right;
		});

	if (it != values.end()) {
		std::cout << "Drop: " << *it << " -> " << *(it + 1) << '\n';
	} else {
		std::cout << "No drop\n";
	}
}
