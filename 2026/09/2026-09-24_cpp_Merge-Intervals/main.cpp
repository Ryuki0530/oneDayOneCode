#include <algorithm>
#include <iostream>
#include <vector>

struct Interval {
	int start;
	int end;
};

std::vector<Interval> mergeIntervals(std::vector<Interval> intervals) {
	std::sort(intervals.begin(), intervals.end(), [](const Interval& a,
													 const Interval& b) {
		return a.start < b.start;
	});

	std::vector<Interval> merged;
	for (const Interval& interval : intervals) {
		if (merged.empty() || interval.start >= merged.back().end) {
			merged.push_back(interval);
		} else {
			merged.back().end = std::max(merged.back().end, interval.end);
		}
	}
	return merged;
}

int main() {
	const std::vector<Interval> input = {
		{1, 3}, {2, 6}, {8, 10}, {9, 12}, {15, 18}
	};
	const std::vector<Interval> expected = {
		{1, 6}, {8, 12}, {15, 18}
	};

	const std::vector<Interval> actual = mergeIntervals(input);
	bool passed = actual.size() == expected.size();
	if (passed) {
		for (std::size_t i = 0; i < expected.size(); ++i) {
			if (actual[i].start != expected[i].start ||
				actual[i].end != expected[i].end) {
				passed = false;
				break;
			}
		}
	}

	std::cout << (passed ? "PASS" : "FAIL") << '\n';
	if (!passed) {
		std::cout << "Expected: ";
		for (const Interval& interval : expected) {
			std::cout << '[' << interval.start << ',' << interval.end << "] ";
		}
		std::cout << "\nActual:   ";
		for (const Interval& interval : actual) {
			std::cout << '[' << interval.start << ',' << interval.end << "] ";
		}
		std::cout << '\n';
	}

	return passed ? 0 : 1;
}
