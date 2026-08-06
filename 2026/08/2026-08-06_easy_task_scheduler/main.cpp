#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

struct Task {
	std::string name;
	std::function<void()> callback;
};

class TaskScheduler {
public:
	void addTask(const std::string& name, std::function<void()> task) {
		tasks_.push_back({name, std::move(task)});
	}

	void run() const {
		for (const auto& task : tasks_) {
			std::cout << "Running: " << task.name << '\n';
			task.callback();
			std::cout << '\n';
		}
	}

	void removeTask(const std::string& name) {
		tasks_.erase(
			std::remove_if(tasks_.begin(), tasks_.end(),
				[&](const Task& task) {
					return task.name == name;
				}),
			tasks_.end());
	}

	void clear() {
		tasks_.clear();
	}

private:
	std::vector<Task> tasks_;
};

int main() {
	TaskScheduler scheduler;

	scheduler.addTask("Hello", []() {
		std::cout << "Hello World!\n";
	});

	scheduler.addTask("Count", []() {
		for (int i = 1; i <= 3; ++i) {
			std::cout << i << '\n';
		}
	});

	scheduler.addTask("Finish", []() {
		std::cout << "Done!\n";
	});

	scheduler.run();

	std::cout << "----- remove Count -----\n";
	scheduler.removeTask("Count");

	scheduler.run();

	std::cout << "----- clear -----\n";
	scheduler.clear();

	scheduler.run();

	return 0;
}