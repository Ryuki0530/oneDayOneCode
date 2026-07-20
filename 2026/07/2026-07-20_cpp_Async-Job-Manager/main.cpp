// Async Job Manager
// C++17

#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <stdexcept>
#include <thread>
#include <vector>

int run_job(int id) {
	thread_local std::mt19937 rng{std::random_device{}()};
	std::uniform_int_distribution<int> sleep_dist(100, 500);

	std::this_thread::sleep_for(std::chrono::milliseconds(sleep_dist(rng)));

	if (id == 2) {
		std::uniform_int_distribution<int> chance_dist(1, 5);
		if (chance_dist(rng) == 1) {
			throw std::runtime_error("Job2 failed");
		}
	}

	return id * id;
}

int main() {
	std::vector<std::future<int>> futures;
	futures.reserve(3);

	for (int id = 1; id <= 3; ++id) {
		futures.emplace_back(std::async(std::launch::async, run_job, id));
	}

	int total = 0;
	for (int id = 1; id <= 3; ++id) {
		try {
			const int result = futures[id - 1].get();
			total += result;
			std::cout << "Job" << id << " = " << result << '\n';
		} catch (const std::exception& ex) {
			std::cout << ex.what() << '\n';
		}
	}

	std::cout << '\n' << "Total = " << total << '\n';
	return 0;
}
