#include <cassert>
#include <iostream>
#include <string_view>

#include "main.cpp"

int main() {
	const std::string_view result = findLongestWord("CAN Ethernet LIN FlexRay");
	std::cout << result << '\n';

	return 0;
}