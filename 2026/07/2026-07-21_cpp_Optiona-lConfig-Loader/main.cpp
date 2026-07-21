#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

std::optional<int> getConfigValue(const std::string& key) {
	static const std::unordered_map<std::string, int> config{{"timeout", 30}, {"retry", 5}};

	const auto it = config.find(key);
	if (it == config.end()) {
		return std::nullopt;
	}

	return it->second;
}

int main() {
	for (const std::string key : {"timeout", "retry", "buffer"}) {
		const auto value = getConfigValue(key);
		if (value.has_value()) {
			std::cout << key << " = " << *value << '\n';
		} else {
			std::cout << key << " = Not Found" << '\n';
		}
	}

	return 0;
}
