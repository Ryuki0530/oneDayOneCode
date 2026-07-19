#include <condition_variable>
#include <chrono>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

class ThreadSafeQueue {
public:
	void push(int value) {
		{
			std::lock_guard<std::mutex> lock(mutex_);
			queue_.push(value);
		}
		condition_.notify_one();
	}

	int pop() {
		std::unique_lock<std::mutex> lock(mutex_);
		condition_.wait(lock, [this] { return !queue_.empty(); });

		int value = queue_.front();
		queue_.pop();
		return value;
	}

private:
	std::queue<int> queue_;
	std::mutex mutex_;
	std::condition_variable condition_;
};

int main() {
	ThreadSafeQueue queue;

	std::thread producer([&queue]() {
		for (int i = 1; i <= 20; ++i) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			queue.push(i);
			std::cout << "Produced: " << i << '\n';
		}
	});

	std::thread consumer([&queue]() {
		for (int i = 0; i < 20; ++i) {
			int value = queue.pop();
			std::cout << "Consumed: " << value << '\n';
		}
	});

	producer.join();
	consumer.join();

	return 0;
}
