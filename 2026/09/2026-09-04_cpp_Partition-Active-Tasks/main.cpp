
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Task {
	std::string name;
	bool completed;
};

int main() {
	std::vector<Task> tasks{
		{"Compile", true},
		{"Test", false},
		{"Lint", true},
		{"Deploy", false},
		{"Package", true}
	};

	const auto middle = std::partition(
		tasks.begin(), tasks.end(),
		[](const Task& task) { return !task.completed; });

	std::cout << "Incomplete:\n";
	for (auto it = tasks.begin(); it != middle; ++it) {
		std::cout << it->name << '\n';
	}

	std::cout << "\nCompleted:\n";
	for (auto it = middle; it != tasks.end(); ++it) {
		std::cout << it->name << '\n';
	}
}
