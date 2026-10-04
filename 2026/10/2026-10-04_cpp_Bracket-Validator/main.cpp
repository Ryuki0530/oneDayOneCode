#include <cassert>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

bool isBalanced(const std::string& text) {
	std::stack<char> brackets;

	for (char c : text) {
		if (c == '(' || c == '[' || c == '{') {
			brackets.push(c);
		} else if (c == ')' || c == ']' || c == '}') {
			if (brackets.empty()) {
				return false;
			}

			const char opening = brackets.top();
			if ((c == ')' && opening != '(') ||
				(c == ']' && opening != '[') ||
				(c == '}' && opening != '{')) {
				return false;
			}
			brackets.pop();
		}
	}

	return brackets.empty();
}

int main() {
	const std::vector<std::string> examples = {
		"(a + b) * [c - {d / e}]",
		"([)]",
		"{[()]}",
		"a + b",
		"((x)"
	};

	for (const std::string& text : examples) {
		std::cout << (isBalanced(text) ? "OK" : "NG") << '\n';
	}

	assert(isBalanced(""));
	assert(!isBalanced(")"));
}
