#include <iostream>
#include <numeric>
#include <vector>

int main() {
	std::vector<int> values{
		12, 5, 8, 21, 14, 3, 10, 7
	};

	const int sum = std::accumulate(
		values.begin(), values.end(), 0,
		[](int total, int value) {
			return total + (value % 2 == 0 ? value : 0);
		});

	std::cout << "Sum: " << sum << '\n';
	return 0;
}
