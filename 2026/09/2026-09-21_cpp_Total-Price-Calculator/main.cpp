#include <iostream>
#include <numeric>
#include <vector>

int main() {
	std::vector<int> prices{
		500,
		1200,
		3000,
		800,
		1500
	};

	const int total = std::accumulate(
		prices.begin(), prices.end(), 0,
		[](int sum, int price) {
			return sum + (price >= 1000 ? price : 0);
		});

	std::cout << "Total: " << total << '\n';
	return 0;
}
