#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class Database {
public:
    void initialize() {
        std::call_once(init_flag_, []() {
            std::cout << "Database initialized.\n";
        });
    }

    void query(int id) {
        initialize();
        std::cout << "Query: " << id << '\n';
    }

private:
    std::once_flag init_flag_;
};

int main() {
    Database db;

    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back([&db, i]() {
            db.query(i);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}