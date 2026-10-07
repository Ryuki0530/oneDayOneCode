#include <deque>
#include <iostream>
#include <string>

int main() {
	std::string text;
	std::deque<std::string> history;
	std::string command;

	while (std::getline(std::cin, command)) {
		if (command.rfind("APPEND ", 0) == 0) {
			history.push_back(text);
			if (history.size() > 3) {
				history.pop_front();
			}
			text += command.substr(7);
		} else if (command == "UNDO") {
			if (!history.empty()) {
				text = history.back();
				history.pop_back();
			}
		} else if (command == "PRINT") {
			std::cout << (text.empty() ? "(empty)" : text) << '\n';
		}
	}

	return 0;
}
