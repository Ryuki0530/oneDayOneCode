
#include <deque>
#include <iomanip>
#include <iostream>

class MovingAverage {
public:
	explicit MovingAverage(std::size_t capacity)
		: capacity_(capacity), sum_(0) {}

	double add(int value) {
		values_.push_back(value);
		sum_ += value;

		if (values_.size() > capacity_) {
			sum_ -= values_.front();
			values_.pop_front();
		}

		return static_cast<double>(sum_) / values_.size();
	}

private:
	std::deque<int> values_;
	std::size_t capacity_;
	long long sum_;
};

//test
int main() {
	MovingAverage filter(3);

	std::cout << std::fixed << std::setprecision(2);
	for (int value : {10, 20, 30, 40, -10}) {
		std::cout << filter.add(value) << '\n';
	}

	return 0;
}
