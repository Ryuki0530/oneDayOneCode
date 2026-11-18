#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

template <typename T>
class ThreadSafeQueue {
public:
	void push(T value) {
		{
			std::lock_guard<std::mutex> lock(mutex_);
			queue_.push(std::move(value));
		}
		condition_.notify_one();
	}

	T pop() {
		std::unique_lock<std::mutex> lock(mutex_);
		condition_.wait(lock, [this] { return !queue_.empty(); });

		T value = std::move(queue_.front());
		queue_.pop();
		return value;
	}

	std::size_t size() const {
		std::lock_guard<std::mutex> lock(mutex_);
		return queue_.size();
	}

private:
	mutable std::mutex mutex_;
	std::condition_variable condition_;
	std::queue<T> queue_;
};

int main() {
	ThreadSafeQueue<int> queue;

	std::thread producer([&queue] {
		for (int value = 1; value <= 10; ++value) {
			queue.push(value);
		}
	});

	std::thread consumer([&queue] {
		for (int i = 0; i < 10; ++i) {
			std::cout << "Consumed: " << queue.pop() << '\n';
		}
	});

	producer.join();
	consumer.join();
	return 0;
}
