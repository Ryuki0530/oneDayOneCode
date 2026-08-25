#include <iostream>
#include <map>
#include <string>

int main() {
	std::map<int, std::string> users = {
		{100, "Alice"},
		{200, "Bob"},
		{300, "Charlie"}
	};

	auto node = users.extract(200);
	if (!node.empty()) {
		if (users.find(250) == users.end()) {
			node.key() = 250;
			users.insert(std::move(node));
		} else {
			users.insert(std::move(node));
		}
	}

	for (const auto& [id, name] : users) {
		std::cout << id << " : " << name << '\n';
	}

	return 0;
}
