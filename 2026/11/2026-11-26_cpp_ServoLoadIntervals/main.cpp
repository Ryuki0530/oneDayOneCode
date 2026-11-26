#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

struct LoadInterval {
	std::size_t start;
	std::size_t end;
	int peakLoad;
};

std::vector<LoadInterval> findLoadIntervals(const std::vector<int>& loads,
											int threshold,
											std::size_t minSamples) {
	std::vector<LoadInterval> intervals;
	bool inInterval = false;
	std::size_t start = 0;
	int peakLoad = 0;

	for (std::size_t i = 0; i < loads.size(); ++i) {
		if (loads[i] >= threshold) {
			if (!inInterval) {
				inInterval = true;
				start = i;
				peakLoad = loads[i];
			} else if (loads[i] > peakLoad) {
				peakLoad = loads[i];
			}
		} else if (inInterval) {
			const std::size_t end = i - 1;
			if (end - start + 1 >= minSamples) {
				intervals.push_back({start, end, peakLoad});
			}
			inInterval = false;
		}
	}

	if (inInterval) {
		const std::size_t end = loads.size() - 1;
		if (end - start + 1 >= minSamples) {
			intervals.push_back({start, end, peakLoad});
		}
	}

	return intervals;
}

int main() {
	const std::vector<int> loads{25, 20, 4, 22, 3, 20, 31, 24};
	const auto intervals = findLoadIntervals(loads, 20, 2);
	for (const LoadInterval& interval : intervals) {
		std::cout << interval.start << ' ' << interval.end << ' '
				  << interval.peakLoad << '\n';
	}

	assert(findLoadIntervals({}, 20, 1).empty());
	const auto singleSample = findLoadIntervals({20}, 20, 1);
	assert(singleSample.size() == 1);
	assert(singleSample[0].start == 0 && singleSample[0].end == 0 &&
		   singleSample[0].peakLoad == 20);
}
