#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class MessageStore {
public:
	void add(const std::string& message) {
		std::unique_lock<std::mutex> lock(mutex_);
		messages_.push_back(message);
	}

	void printLastMessage() {
		std::string lastMessage;

		std::unique_lock<std::mutex> lock(mutex_);
		if (messages_.empty()) {
			lock.unlock();
			std::cout << "No messages" << std::endl;
			return;
		}

		lastMessage = messages_.back();
		lock.unlock();

		std::cout << lastMessage << std::endl;
	}

private:
	std::vector<std::string> messages_;
	std::mutex mutex_;
};

int main() {
	MessageStore store;

	std::thread t1([&]() {
		store.add("Hello");
	});

	std::thread t2([&]() {
		store.add("World");
	});

	t1.join();
	t2.join();

	store.printLastMessage();

	return 0;
}
