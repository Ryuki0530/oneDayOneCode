#include <cassert>
#include <iostream>
#include <vector>

std::vector<int> spiralOrder(const std::vector<std::vector<int>>& matrix) {
	if (matrix.empty() || matrix.front().empty()) {
		return {};
	}

	std::vector<int> result;
	result.reserve(matrix.size() * matrix.front().size());

	int top = 0;
	int bottom = static_cast<int>(matrix.size()) - 1;
	int left = 0;
	int right = static_cast<int>(matrix.front().size()) - 1;

	while (top <= bottom && left <= right) {
		for (int column = left; column <= right; ++column) {
			result.push_back(matrix[top][column]);
		}
		++top;

		for (int row = top; row <= bottom; ++row) {
			result.push_back(matrix[row][right]);
		}
		--right;

		if (top <= bottom) {
			for (int column = right; column >= left; --column) {
				result.push_back(matrix[bottom][column]);
			}
			--bottom;
		}

		if (left <= right) {
			for (int row = bottom; row >= top; --row) {
				result.push_back(matrix[row][left]);
			}
			++left;
		}
	}

	return result;
}

//test cases
int main() {
	const std::vector<std::vector<int>> matrix1 = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12}
	};
	const std::vector<std::vector<int>> matrix2 = {{1, 2, 3}};
	const std::vector<std::vector<int>> matrix3 = {{1}, {2}, {3}};

	assert((spiralOrder(matrix1) == std::vector<int>{1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7}));
	assert((spiralOrder(matrix2) == std::vector<int>{1, 2, 3}));
	assert((spiralOrder(matrix3) == std::vector<int>{1, 2, 3}));
	assert(spiralOrder({}).empty());
	assert(spiralOrder({{}}).empty());
	assert((spiralOrder({{42}}) == std::vector<int>{42}));

	for (int value : spiralOrder(matrix1)) {
		std::cout << value << ' ';
	}
	std::cout << '\n';
}
