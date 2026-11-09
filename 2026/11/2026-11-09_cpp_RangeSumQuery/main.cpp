#include <iostream>
#include <optional>
#include <vector>

class RangeSumQuery {
public:
	explicit RangeSumQuery(const std::vector<int>& values)
		: prefix_(values.size() + 1, 0) {
		for (std::size_t i = 0; i < values.size(); ++i) {
			prefix_[i + 1] = prefix_[i] + values[i];
		}
	}

	std::optional<long long> query(int left, int right) const {
		if (left < 0 || right < 0 || left > right ||
			right >= static_cast<int>(prefix_.size()) - 1) {
			return std::nullopt;
		}

		return prefix_[right + 1] - prefix_[left];
	}

private:
	std::vector<long long> prefix_;
};

int main() {
	const std::vector<int> values = {3, -2, 5, 1, -4, 6};
	const RangeSumQuery range_sum(values);

	const std::vector<std::pair<int, int>> queries = {
		{0, 2}, {2, 5}, {3, 3}, {0, 5},
		{-1, 2}, {4, 2}, {0, 6}
	};

	for (const auto& [left, right] : queries) {
		const auto result = range_sum.query(left, right);
		if (result.has_value()) {
			std::cout << *result << '\n';
		} else {
			std::cout << "INVALID\n";
		}
	}

	return 0;
}
