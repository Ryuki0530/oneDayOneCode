// main.cpp
#include <iostream>
#include <string>
#include <queue>
#include <sstream>
#include <chrono>
#include <iomanip>

int main() {
    long long W;
    std::cin >> W;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::queue<long long> timestamps;
    std::string line;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string date_str, time_str, path;
        if (!(iss >> date_str >> time_str >> path)) {
            continue;
        }

        int year  = std::stoi(date_str.substr(0, 4));
        int month = std::stoi(date_str.substr(5, 2));
        int day   = std::stoi(date_str.substr(8, 2));

        int hour   = std::stoi(time_str.substr(0, 2));
        int minute = std::stoi(time_str.substr(3, 2));
        int second = std::stoi(time_str.substr(6, 2));

        auto days_from_civil = [](int y, unsigned m, unsigned d) -> long long {
            // 1970-01-01 を 0 とする日数
            y -= (m <= 2);
            const int era = (y >= 0 ? y : y - 399) / 400;
            const unsigned yoe = static_cast<unsigned>(y - era * 400);      // [0, 399]
            const unsigned doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;  // [0, 365]
            const unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;     // [0, 146096]
            return static_cast<long long>(era) * 146097LL + static_cast<long long>(doe) - 719468LL;
        };

        long long days = days_from_civil(year, static_cast<unsigned>(month), static_cast<unsigned>(day));
        long long current_timestamp = days * 86400LL + hour * 3600LL + minute * 60LL + second;

        long long window_start = current_timestamp - W + 1;
        while (!timestamps.empty() && timestamps.front() < window_start) {
            timestamps.pop();
        }

        timestamps.push(current_timestamp);

        std::cout << date_str << " " << time_str << " " << path << " " << timestamps.size() << "\n";
    }gi

    return 0;
}
