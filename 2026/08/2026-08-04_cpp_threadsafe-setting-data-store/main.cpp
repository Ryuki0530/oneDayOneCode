#include <iostream>
#include <unordered_map>
#include <shared_mutex>
#include <mutex>
#include <optional>
#include <thread>
#include <chrono>
#include <string>

class ConfigStore {
private:
    mutable std::shared_mutex mutex_;
    std::unordered_map<std::string, int> data_;

public:
    void set(const std::string& key, int value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        data_[key] = value;
    }

    std::optional<int> get(const std::string& key) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        auto it = data_.find(key);
        if (it != data_.end()) {
            return it->second;
        }
        return std::nullopt;
    }

    bool remove(const std::string& key) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        return data_.erase(key) > 0;
    }
};

int main() {
    ConfigStore store;

    // Writer thread: sets volume from 0 to 9
    std::thread writer([&store]() {
        for (int i = 0; i < 10; ++i) {
            store.set("volume", i);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    // Reader thread 1
    std::thread reader1([&store]() {
        for (int i = 0; i < 10; ++i) {
            auto value = store.get("volume");
            if (value) {
                std::cout << "Reader 1: volume = " << value.value() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(80));
        }
    });

    // Reader thread 2
    std::thread reader2([&store]() {
        for (int i = 0; i < 10; ++i) {
            auto value = store.get("volume");
            if (value) {
                std::cout << "Reader 2: volume = " << value.value() << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(120));
        }
    });

    writer.join();
    reader1.join();
    reader2.join();

    auto final_value = store.get("volume");
    if (final_value) {
        std::cout << "Final value: " << final_value.value() << std::endl;
    }

    return 0;
}
