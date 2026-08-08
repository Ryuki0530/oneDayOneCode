#include <iostream>
#include <string_view>

void printLog(std::string_view log)
{
	const std::size_t pos = log.find(':');
	if (pos == std::string_view::npos) {
		std::cout << "Invalid log\n\n";
		return;
	}

	const std::string_view level = log.substr(0 , pos);
	const std::string_view message = log.substr(1 + pos);

	std::cout << "Level\t: " << level << '\n';
	std::cout << "Message\t: " << message << '\n' << '\n';
}

int main()
{
	printLog("INFO:Robot started");
	printLog("ERROR:Motor timeout");
	printLog("WARN:Battery low");
}
