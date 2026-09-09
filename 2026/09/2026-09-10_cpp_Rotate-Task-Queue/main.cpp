#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct Task {
    std::string name;
    bool active;
};

int main() {
    std::vector<Task> tasks{
        {"Compile", false},
        {"Test", false},
        {"Deploy", true},
        {"Monitor", false},
        {"Cleanup", false}
    };

    const auto activeTask = std::find_if(
        tasks.begin(), tasks.end(),
        [](const Task& task) { return task.active; });

    if (activeTask != tasks.end()) {
        std::rotate(tasks.begin(), activeTask, tasks.end());
    }

    for (const auto& task : tasks) {
        std::cout << task.name << '\n';
    }
}