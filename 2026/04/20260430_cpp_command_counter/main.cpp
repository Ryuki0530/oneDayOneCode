#include <iostream>
#include <string>
#include <unordered_map>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N;
    std::cin >> N;

    std::unordered_map<std::string, int> count;
    count["ADD"] = 0;
    count["DELETE"] = 0;
    count["UPDATE"] = 0;
    count["READ"] = 0;

    for (int i = 0; i < N; ++i) {
        std::string command;
        std::cin >> command;
        ++count[command];
    }

    std::cout << "ADD " << count["ADD"] << '\n';
    std::cout << "DELETE " << count["DELETE"] << '\n';
    std::cout << "UPDATE " << count["UPDATE"] << '\n';
    std::cout << "READ " << count["READ"] << '\n';

    return 0;
}