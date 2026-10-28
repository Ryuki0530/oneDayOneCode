#include <algorithm>
#include <iostream>
#include <vector>

void rotateRight(std::vector<int>& values, int k) {
	if (values.empty()) {
		return;
	}

	k %= static_cast<int>(values.size());
	if (k == 0) {
		return;
	}

	std::rotate(values.begin(), values.end() - k, values.end());
}

void printValues(const std::vector<int>& values) {
	for (int value : values) {
		std::cout << value << ' ';
	}
	std::cout << '\n';
}

int main() {
	std::vector<int> values = {1, 2, 3, 4, 5};

	rotateRight(values, 2);
	printValues(values); // 4 5 1 2 3

	values = {1, 2, 3, 4, 5};
	rotateRight(values, 0);
	printValues(values); // 1 2 3 4 5

	values = {1, 2, 3, 4, 5};
	rotateRight(values, 7);
	printValues(values); // 4 5 1 2 3

	values.clear();
	rotateRight(values, 2);
	printValues(values); // empty

	return 0;
}
