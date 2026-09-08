#include <algorithm>
#include <iostream>
#include <vector>

int main() {
	std::vector<int> values{5, 2, 8, 5, 1, 5, 3, 8, 8, 4};
	int target = 8;

	std::sort(values.begin(), values.end());
	const auto range = std::equal_range(values.begin(), values.end(), target);

	if (range.first == range.second) {
		std::cout << "Not found\n";
		return 0;
	}

	const auto firstIndex = std::distance(values.begin(), range.first);
	const auto lastIndex = std::distance(values.begin(), range.second) - 1;
	const auto count = std::distance(range.first, range.second);

	std::cout << "First index: " << firstIndex << '\n'
			  << "Last index: " << lastIndex << '\n'
			  << "Count: " << count << '\n';
}
