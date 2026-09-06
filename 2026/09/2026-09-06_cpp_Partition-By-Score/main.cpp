#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Student {
	std::string name;
	int score;
};

int main() {
	std::vector<Student> students{
		{"Alice", 82},
		{"Bob", 45},
		{"Charlie", 71},
		{"Dave", 58},
		{"Eve", 90}
	};

	const auto boundary = std::partition(
		students.begin(),
		students.end(),
		[](const Student& student) {
			return student.score >= 60;
		});

	std::cout << "Passed:\n";
	for (auto it = students.begin(); it != boundary; ++it) {
		std::cout << it->name << ": " << it->score << '\n';
	}

	std::cout << "\nFailed:\n";
	for (auto it = boundary; it != students.end(); ++it) {
		std::cout << it->name << ": " << it->score << '\n';
	}
}
