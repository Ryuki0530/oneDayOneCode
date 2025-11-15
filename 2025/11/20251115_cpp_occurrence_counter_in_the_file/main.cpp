#include <bits/stdc++.h>
using namespace std;

static inline string trim(const string& s) {
    size_t first = 0, last = s.size();
    while (first < last && isspace(static_cast<unsigned char>(s[first]))) ++first;
    while (last > first && isspace(static_cast<unsigned char>(s[last - 1]))) --last;
    return s.substr(first, last - first);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string filename;
    if (!getline(cin, filename)) {
        cerr << "Failed to read filename from stdin.\n";
        return 1;
    }
    filename = trim(filename);
    if (filename.empty()) {
        cerr << "Filename is empty.\n";
        return 1;
    }

    ifstream fin(filename);
    if (!fin) {
        cerr << "Failed to open file: " << filename << "\n";
        return 1;
    }

    unordered_map<string, long long> freq;
    string line, token;
    while (getline(fin, line)) {
        token.clear();
        for (unsigned char uc : line) {
            if (isalnum(uc)) {
                token.push_back(static_cast<char>(tolower(uc)));
            } else {
                if (!token.empty()) {
                    ++freq[token];
                    token.clear();
                }
            }
        }
        if (!token.empty()) {
            ++freq[token];
            token.clear();
        }
    }

    vector<pair<string, long long>> items;
    items.reserve(freq.size());
    for (auto& kv : freq) items.emplace_back(kv.first, kv.second);

    sort(items.begin(), items.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) return a.second > b.second;   // count desc
        return a.first < b.first;                               // word asc
    });

    for (const auto& [word, count] : items) {
        cout << word << ' ' << count << '\n';
    }
    return 0;
}