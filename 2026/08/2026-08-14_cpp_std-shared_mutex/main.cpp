#include <iostream>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

class ConfigStore {
public:
	void set(const std::string& key, const std::string& value) {
		std::unique_lock<std::shared_mutex> lock(mutex_);
		values_[key] = value;
	}

	std::optional<std::string> get(const std::string& key) const {
		std::shared_lock<std::shared_mutex> lock(mutex_);
		const auto it = values_.find(key);
		if (it == values_.end()) {
			return std::nullopt;
		}
		return it->second;
	}

private:
	mutable std::shared_mutex mutex_;
	std::unordered_map<std::string, std::string> values_;
};

int main() {
	ConfigStore store;
	store.set("mode", "debug");

	std::vector<std::thread> threads;
	for (int i = 1; i <= 3; ++i) {
		threads.emplace_back([&store, i]() {
			const auto value = store.get("mode");
			if (value) {
				std::cout << "Reader " << i << ": " << *value << '\n';
			}
		});
	}

	for (auto& thread : threads) {
		thread.join();
	}

	if (!store.get("timeout")) {
		std::cout << "timeout was not found" << '\n';
	}

	return 0;
}
