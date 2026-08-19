#include <atomic>
#include <vector>
#include <thread>
#include <iostream>
const int NUM_THREADS = 4;
const int LIMIT = 100000;

class limited_counter {
public:
    limited_counter(int limit)
        : value(0), limit(limit) {
    }

    limited_counter(const limited_counter&) = delete;
    limited_counter& operator=(const limited_counter&) = delete;

    bool increment() {
        int current = value.load();

        while (current < limit) {
            if (value.compare_exchange_weak(current, current + 1)) {
                return true;
            }
        }

        return false;
    }

    int get() const {
        return value.load();
    }

private:
    std::atomic<int> value;
    int limit;
};

void thread_process(limited_counter& counter) {
    for (int j = 0; j < 50000; ++j) {
        counter.increment();
    }
}

int main(){
    limited_counter counter(LIMIT);
    std::vector<std::thread> threads;

    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(thread_process, std::ref(counter));
    }

    for(auto& t : threads){
        t.join();
    }
    std::cout << "Counter: " << counter.get() << std::endl;
}


