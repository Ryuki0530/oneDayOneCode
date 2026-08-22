#include <algorithm>
#include <iostream>
#include <vector>

class NumberList {
public:
	void add(int value) {
		numbers_.push_back(value);
	}

	void remove_if_greater_than(int threshold) {
		numbers_.erase(
			std::remove_if(numbers_.begin(), numbers_.end(),
						   [threshold](int value) { return value > threshold; }),
			numbers_.end());
	}

	void print() const {
		for (std::size_t i = 0; i < numbers_.size(); ++i) {
			if (i > 0) {
				std::cout << ' ';
			}
			std::cout << numbers_[i];
		}
		std::cout << '\n';
	}

private:
	std::vector<int> numbers_;
};

int main() {
	NumberList numbers;

	numbers.add(3);
	numbers.add(12);
	numbers.add(7);
	numbers.add(20);
	numbers.add(5);

	numbers.remove_if_greater_than(10);
	numbers.print();

	return 0;
}
