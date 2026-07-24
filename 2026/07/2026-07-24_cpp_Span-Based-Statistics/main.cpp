#include <array>
#include <iostream>
#include <span>
#include <vector>

struct Stats {
	int min;
	int max;
	double average;
};

Stats calculate(std::span<const int> values) {
	int minValue = values.front();
	int maxValue = values.front();
	long long sum = 0;

	for (int value : values) {
		if (value < minValue) {
			minValue = value;
		}
		if (value > maxValue) {
			maxValue = value;
		}
		sum += value;
	}

	return {minValue, maxValue, static_cast<double>(sum) / values.size()};
}

int main() {
	std::vector<int> v{3, 8, 1, 5};
	auto s = calculate(v);

	std::cout << "min = " << s.min << '\n';
	std::cout << "max = " << s.max << '\n';
	std::cout << "average = " << s.average << '\n';
	return 0;
}
