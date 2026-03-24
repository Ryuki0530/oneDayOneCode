#include <iostream>
#include <map>
#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>

int main() {
    std::map<std::string, int> wordCount;
    std::string line, word;
    while (std::getline(std::cin, line)) {
        std::istringstream stream(line);
        while (stream >> word) {
            std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) {
                return std::tolower(c);
            });
            wordCount[word]++;
        }
    }

    for (const auto& pair : wordCount) {
        std::cout << pair.first << " " << pair.second << std::endl;
    }

    return 0;
}