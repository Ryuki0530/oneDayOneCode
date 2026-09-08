 #include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Result {
	std::string name;
	int score;
};

int main() {
	std::vector<Result> results{
		{"Alice", 78},
		{"Bob", 92},
		{"Charlie", 85},
		{"Dave", 67},
		{"Eve", 95},
		{"Frank", 88}
	};

	std::vector<Result> top_results(3);

	std::partial_sort_copy(
		results.begin(),
		results.end(),
		top_results.begin(),
		top_results.end(),
		[](const Result& left, const Result& right) {
			return left.score > right.score;
		}
	);

	for (const auto& result : top_results) {
		std::cout << result.name << ": " << result.score << '\n';
	}
}
