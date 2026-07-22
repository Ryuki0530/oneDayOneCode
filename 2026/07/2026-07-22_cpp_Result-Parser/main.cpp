#include <iostream>
#include <string>

enum class ParseError {
	EmptyInput,
	InvalidNumber
};

struct ParseResult {
	bool success;
	int value;
	ParseError error;
};

ParseResult parseInt(const std::string& input) {
	if (input.empty()) {
		return {false, 0, ParseError::EmptyInput};
	}

	try {
		size_t pos = 0;
		int value = std::stoi(input, &pos);
		if (pos != input.size()) {
			return {false, 0, ParseError::InvalidNumber};
		}
		return {true, value, ParseError::InvalidNumber};
	} catch (...) {
		return {false, 0, ParseError::InvalidNumber};
	}
}

const char* toErrorMessage(ParseError error) {
	switch (error) {
		case ParseError::EmptyInput:
			return "Empty Input";
		case ParseError::InvalidNumber:
			return "Invalid Number";
		default:
			return "Unknown Error";
	}
}

int main() {
	const std::string samples[] = {"123", "", "abc", "-42"};

	for (const auto& s : samples) {
		ParseResult result = parseInt(s);
		if (result.success) {
			std::cout << "Success: " << result.value << '\n';
		} else {
			std::cout << "Error: " << toErrorMessage(result.error) << '\n';
		}
	}

	return 0;
}
