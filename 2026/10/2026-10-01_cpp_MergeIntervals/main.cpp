 #include <algorithm>
 #include <iostream>
 #include <vector>

 struct Interval {
	 int start;
	 int end;
 };

 std::vector<Interval> mergeIntervals(std::vector<Interval> intervals) {
	 if (intervals.empty()) {
		 return {};
	 }

	 std::sort(intervals.begin(), intervals.end(), [](const Interval& lhs, const Interval& rhs) {
		 return lhs.start < rhs.start;
	 });

	 std::vector<Interval> merged;
	 merged.push_back(intervals.front());

	 for (std::size_t i = 1; i < intervals.size(); ++i) {
		 Interval& current = merged.back();
		 const Interval& next = intervals[i];

		 if (next.start <= current.end) {
			 current.end = std::max(current.end, next.end);
		 } else {
			 merged.push_back(next);
		 }
	 }

	 return merged;
 }

 int main() {
	 std::vector<Interval> intervals{
		 {8, 10},
		 {1, 3},
		 {15, 18},
		 {2, 6},
		 {10, 12}
	 };

	 const std::vector<Interval> merged = mergeIntervals(intervals);
	 for (const Interval& interval : merged) {
		 std::cout << '[' << interval.start << ", " << interval.end << "]\n";
	 }

	 return 0;
 }
