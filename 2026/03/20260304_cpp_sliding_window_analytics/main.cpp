#include <iostream>
#include <deque>
#include <unordered_map>
#include <map>
#include <queue>
#include <set>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

struct Event {
    long long t;
    string user;
    string type;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int Q;
    long long W;
    cin >> Q >> W;
    cin.ignore();

    deque<Event> events;
    unordered_map<string, int> userCount, typeCount;
    int totalCount = 0;

    // For TOP queries: lazy deletion with map（今回は未使用）
    map<int, set<string>> userByCount, typeByCount;
    unordered_map<string, int> userCurrentCount, typeCurrentCount;

    auto cleanup = [&](long long currentTime) {
        // CHANGED:
        // もともとの (T-W, T] なら「<= currentTime - W」を消すのが正しい
        // 例の出力（COUNT 7 が 3）に合わせる [T-W, T] にするなら「< currentTime - W」
        while (!events.empty() && events.front().t < currentTime - W) { // CHANGED: <= -> <
            Event e = events.front();
            events.pop_front();

            userCount[e.user]--;
            typeCount[e.type]--;
            totalCount--;

            if (userCount[e.user] == 0) userCount.erase(e.user);
            if (typeCount[e.type] == 0) typeCount.erase(e.type);
        }
    };

    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;

        istringstream iss(line);
        string cmd;
        iss >> cmd;

        if (cmd == "ADD") {
            long long t;
            string user, type;
            iss >> t >> user >> type;

            cleanup(t);
            events.push_back({t, user, type});
            userCount[user]++;
            typeCount[type]++;
            totalCount++;

        } else if (cmd == "COUNT") {
            long long t;
            iss >> t;

            cleanup(t);
            cout << totalCount << "\n";

        } else if (cmd == "TOPUSER") {
            long long t;
            int k;
            iss >> t >> k;

            cleanup(t);

            vector<pair<int, string>> sorted;
            sorted.reserve(userCount.size());
            for (auto& p : userCount) {
                sorted.push_back({p.second, p.first});
            }
            sort(sorted.begin(), sorted.end(),
                 [](const auto& a, const auto& b) {
                     if (a.first != b.first) return a.first > b.first;
                     return a.second < b.second;
                 });

            for (int i = 0; i < min(k, (int)sorted.size()); i++) {
                cout << sorted[i].second << " " << sorted[i].first << "\n";
            }

        } else if (cmd == "TOPTYPE") {
            long long t;
            int k;
            iss >> t >> k;

            cleanup(t);

            vector<pair<int, string>> sorted;
            sorted.reserve(typeCount.size());
            for (auto& p : typeCount) {
                sorted.push_back({p.second, p.first});
            }
            sort(sorted.begin(), sorted.end(),
                 [](const auto& a, const auto& b) {
                     if (a.first != b.first) return a.first > b.first;
                     return a.second < b.second;
                 });

            for (int i = 0; i < min(k, (int)sorted.size()); i++) {
                cout << sorted[i].second << " " << sorted[i].first << "\n";
            }
        }
    }

    return 0;
}