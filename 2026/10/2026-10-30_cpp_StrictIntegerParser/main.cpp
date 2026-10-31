#include <charconv>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

std::optional<int> parseInteger(const std::string& text) {
	if (text.empty()) {
		return std::nullopt;
	}

	int value{};
	const auto result = std::from_chars(
		text.data(), text.data() + text.size(), value);

	if (result.ec != std::errc{} || result.ptr != text.data() + text.size()) {
		return std::nullopt;
	}

	return value;
}

int main() {
	const std::vector<std::string> inputs{
		"42",
		"-17",
		"007",
		"12abc",
		"",
		" 8",
		"+5",
		"-",
		"999999999999999999999999",
		"0",
		"-0",
	};

	for (const std::string& input : inputs) {
		std::cout << '"' << input << "\" -> ";
		if (const auto value = parseInteger(input)) {
			std::cout << *value;
		} else {
			std::cout << "INVALID";
		}
		std::cout << '\n';
	}
}
