
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

class Config {
public:
	void set(const std::string& key, int value) {
		settings_[key] = value;
	}

	std::optional<int> get(const std::string& key) const {
		const auto it = settings_.find(key);
		if (it == settings_.end()) {
			return std::nullopt;
		}
		return it->second;
	}

	int get_or(const std::string& key, int default_value) const {
		const auto value = get(key);
		return value.value_or(default_value);
	}

private:
	std::unordered_map<std::string, int> settings_;
};

int main() {
	Config config;

	config.set("port", 8080);
	config.set("timeout", 30);

	const auto port = config.get("port");
	const auto timeout = config.get("timeout");
	const auto retry = config.get("retry");

	if (port) {
		std::cout << "port = " << *port << '\n';
	}

	if (timeout) {
		std::cout << "timeout = " << *timeout << '\n';
	}

	if (retry) {
		std::cout << "retry = " << *retry << '\n';
	} else {
		std::cout << "retry = not found\n";
	}

	std::cout << config.get_or("retry", 3) << '\n';

	return 0;
}
