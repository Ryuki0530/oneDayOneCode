#include <iostream>
#include <deque>
#include <vector>
#include <algorithm>

class MedianFilter {
public:
    explicit MedianFilter(std::size_t n) : window_size(n) {}

    void add(int value) {
        buffer.push_back(value);
        if (buffer.size() > window_size) {
            buffer.pop_front();
        }
    }

    int get_median() const {
        if (buffer.empty()) {
            return 0;
        }
        std::vector<int> sorted(buffer.begin(), buffer.end());
        std::sort(sorted.begin(), sorted.end());
        return sorted[sorted.size() / 2];
    }

private:
    std::size_t window_size;
    std::deque<int> buffer;
};

int main() {
    std::size_t n;
    if (!(std::cin >> n) || n == 0) {
        return 0;
    }
    MedianFilter filter(n);
    int value;
    while (std::cin >> value) {
        filter.add(value);
        std::cout << filter.get_median() << '\n';
    }
    return 0;
}