#include <any>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>

class Config {
public:
	template <typename T>
	void set(const std::string& key, T value) {
		values_[key] = std::any(std::move(value));
	}

	template <typename T>
	T get(const std::string& key) const {
		const auto it = values_.find(key);
		if (it == values_.end()) {
			throw std::runtime_error("Key not found: " + key);
		}

		try {
			return std::any_cast<T>(it->second);
		} catch (const std::bad_any_cast&) {
			throw std::runtime_error("Type mismatch for key: " + key);
		}
	}

	bool contains(const std::string& key) const {
		return values_.find(key) != values_.end();
	}

	void remove(const std::string& key) {
		values_.erase(key);
	}

private:
	std::unordered_map<std::string, std::any> values_;
};

int main() {
	Config config;

	config.set<std::string>("name", "Alice");
	config.set<int>("age", 20);
	config.set<double>("pi", 3.14159);

	std::cout << "name = " << config.get<std::string>("name") << '\n';
	std::cout << "age = " << config.get<int>("age") << '\n';
	std::cout << "pi = " << config.get<double>("pi") << '\n';

	return 0;
}
