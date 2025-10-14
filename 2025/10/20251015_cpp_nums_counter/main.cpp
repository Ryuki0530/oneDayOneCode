#include <iostream>
#include <map>

int main() {
    int N;
    std::cin >> N;
    std::map<int, int> counter;
    for (int itr = 0; itr <N;itr ++){
        int num;
        std::cin >> num;
        counter[num]++;
    }
    for (const auto& p : counter) {
        std::cout << p.first << ": " << p.second << std::endl;
    }
    return 0;
}