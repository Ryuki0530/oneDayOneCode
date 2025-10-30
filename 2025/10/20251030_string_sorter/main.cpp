#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>

int main() {
    std::string line;
    if (!std::getline(std::cin, line)) {
        return 0;
    }

    std::transform(line.begin(), line.end(), line.begin(),
                   [](unsigned char ch) { return static_cast<char>(std::tolower(ch)); });

    std::istringstream iss(line);
    std::vector<std::string> words;
    std::string word;
    while (iss >> word) {
        words.push_back(word);
    }

    std::sort(words.begin(), words.end());

    for (std::size_t i = 0; i < words.size(); ++i) {
        if (i) {
            std::cout << ' ';
        }
        std::cout << words[i];
    }
    std::cout << '\n';

    return 0;
}