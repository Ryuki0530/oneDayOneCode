#include <iostream>
#include <unordered_set>
#include <vector>

int firstMissingPositive(const std::vector<int>& numbers) {
	std::unordered_set<int> positives;

	for (int number : numbers) {
		if (number > 0) {
			positives.insert(number);
		}
	}

	int candidate = 1;
	while (positives.count(candidate) != 0) {
		++candidate;
	}

	return candidate;
}

int main() {
	const std::vector<std::vector<int>> examples = {
		{3, 4, -1, 1},
		{1, 2, 0},
		{2, 2, 5, -3},
		{}
	};

	for (const auto& numbers : examples) {
		std::cout << firstMissingPositive(numbers) << '\n';
	}

	return 0;
}
