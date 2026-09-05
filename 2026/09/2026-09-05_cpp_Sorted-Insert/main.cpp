#include <algorithm>
#include <iostream>
#include <vector>

class SortedNumbers {
private:
	std::vector<int> numbers;

public:
	void insert(int value) {
		auto position = std::lower_bound(numbers.begin(), numbers.end(), value);
		numbers.insert(position, value);
	}

	void print() const {
		for (std::size_t i = 0; i < numbers.size(); ++i) {
			if (i > 0) {
				std::cout << ' ';
			}
			std::cout << numbers[i];
		}
		std::cout << '\n';
	}
};

int main() {
	SortedNumbers numbers;

	numbers.insert(30);
	numbers.insert(10);
	numbers.insert(20);
	numbers.insert(20);
	numbers.insert(5);

	numbers.print();
}
