#include<iostream>
#include<string>
#include<charconv>
#include<vector>

void printParseResult(const std::string& text){
    int value;
    const auto result =  std::from_chars(text.data(), text.data() + text.size(), value, 10);

    std::cout << '"' << text << "\" -> ";
    
    if (result.ec == std::errc::invalid_argument) {
        std::cout << "INVALID" << std::endl;
        return;
    }
    if (result.ec == std::errc::result_out_of_range) {
        std::cout << "OUT_OF_RANGE" << std::endl;
        return;
    }
    if (result.ptr != text.data() + text.size()) {
        std::cout << "TRAILING_CHARACTERS" << std::endl;
        return;
    }
    std::cout << "OK: " << value << '\n';
}

int main() {
	const std::vector<std::string> inputs = {
		"42", "-17", "003", "12abc", " 42", "+5", "",
		"999999999999999999999999"
	};
	for (const auto& input : inputs) {
		printParseResult(input);
	}
}

