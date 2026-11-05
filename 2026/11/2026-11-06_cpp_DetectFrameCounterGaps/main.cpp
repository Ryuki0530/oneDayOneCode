#include <iostream>
#include <unordered_map>
#include <vector>

struct Frame {
	int sourceId;
	int counter;
};

int main() {
	const std::vector<Frame> frames{
		{0x100, 14},
		{0x200, 3},
		{0x100, 15},
		{0x100, 0},
		{0x200, 5},
		{0x100, 0},
		{0x200, 6},
	};

	std::unordered_map<int, int> lastCounter;

	for (const Frame& frame : frames) {
		const auto [it, isFirst] = lastCounter.emplace(frame.sourceId, frame.counter);
		if (isFirst) {
			continue;
		}

		const int expected = (it->second + 1) % 16;
		if (frame.counter == it->second) {
			std::cout << "DUPLICATE 0x" << std::hex << frame.sourceId
					  << std::dec << " counter=" << frame.counter << '\n';
		} else if (frame.counter != expected) {
			std::cout << "GAP 0x" << std::hex << frame.sourceId
					  << std::dec << " expected=" << expected
					  << " actual=" << frame.counter << '\n';
		}

		it->second = frame.counter;
	}
}
