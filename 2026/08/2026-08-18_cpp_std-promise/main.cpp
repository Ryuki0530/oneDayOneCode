#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>

void calculateSum(int begin, int end, std::promise<long long> result) {
    try {
        if (begin > end) {
            throw std::invalid_argument("begin must be <= end");
        }

        long long sum = 0;
        for (int i = begin; i <= end; ++i) {
            sum += static_cast<long long>(i);
        }

        result.set_value(sum);
    } catch (...) {
        result.set_exception(std::current_exception());
    }
}

int main() {
    std::promise<long long> promise;
    auto future = promise.get_future();

    std::cout << "Calculating..." << std::endl;

    std::thread worker(calculateSum, 1, 1000000, std::move(promise));

    try {
        const long long sum = future.get();
        std::cout << "Total: " << sum << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    worker.join();

    return 0;
}
