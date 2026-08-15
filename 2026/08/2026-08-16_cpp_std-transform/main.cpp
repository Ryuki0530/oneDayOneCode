#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
	const std::vector<int> values = {1, 2, 3, 4, 5};
	std::vector<int> squared(values.size());

	std::transform(values.begin(), values.end(), squared.begin(),
				   [](int value) { return value * value; });

	std::copy(squared.begin(), squared.end(),
			  std::ostream_iterator<int>(std::cout, " "));

	return 0;
}
