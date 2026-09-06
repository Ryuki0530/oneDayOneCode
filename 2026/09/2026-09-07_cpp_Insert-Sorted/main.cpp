#include <algorithm>
#include <iostream>
#include <vector>

void insertSorted(std::vector<int>& values, int value) {
    const auto position = std::lower_bound(values.begin(), values.end(), value);
    values.insert(position, value);
}

int main() {
    std::vector<int> values{10, 20, 30, 40, 50};

    insertSorted(values, 25);
    insertSorted(values, 10);
    insertSorted(values, 60);
    insertSorted(values, 35);

    for (int value : values) {
        std::cout << value << ' ';
    }
    std::cout << '\n';
}