 #include <algorithm>
#include <iostream>
#include <vector>

int main() {
	std::vector<int> raw_values = {120, 80, 200, 50, 150};
	std::vector<int> normalized_values(raw_values.size());

	std::transform(raw_values.begin(), raw_values.end(), normalized_values.begin(),
				   [](int value) { return value - 100; });

	for (int value : normalized_values) {
		std::cout << value << '\n';
	}

	return 0;
}
