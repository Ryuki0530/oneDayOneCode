#include <condition_variable>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

class BlockingQueue {
public:
    explicit BlockingQueue(std::size_t capacity)
        : capacity_(capacity) {
    }

    void push(int value) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [&] { return queue_.size() < capacity_; });
        queue_.push(value);
        lock.unlock();
        not_empty_.notify_one();
    }

    int pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [&] { return !queue_.empty(); });
        int value = queue_.front();
        queue_.pop();
        lock.unlock();
        not_full_.notify_one();
        return value;
    }

private:
    std::queue<int> queue_;
    const std::size_t capacity_;
    std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
};

void producer(BlockingQueue& queue) {
    for (int value = 1; value <= 10; ++value) {
        queue.push(value);
        std::cout << "Produced: " << value << '\n';
    }
}

void consumer(BlockingQueue& queue) {
    for (int i = 0; i < 10; ++i) {
        const int value = queue.pop();
        std::cout << "Consumed: " << value << '\n';
    }
}

int main() {
    BlockingQueue queue(3);

    std::thread producer_thread(producer, std::ref(queue));
    std::thread consumer_thread(consumer, std::ref(queue));

    producer_thread.join();
    consumer_thread.join();
}