#include <bits/stdc++.h>
using namespace std;

class ConfigChecker {
public:
    ConfigChecker(istream &in, ostream &out) : in_(in), out_(out) {}

    int run() {
        int N;
        if (!(in_ >> N)) return 0;
        string line;
        getline(in_, line); // consume endline

        for (int i = 0; i < N; ++i) {
            if (!getline(in_, line)) break;
            string key = extract_key(line);
            if (seen_.count(key)) {
                if (!reported_.count(key)) {
                    duplicates_.push_back(key);
                    reported_.insert(key);
                }
            } else {
                seen_.insert(key);
            }
        }

        if (duplicates_.empty()) {
            out_ << "OK\n";
        } else {
            for (auto &k : duplicates_) out_ << k << '\n';
        }
        return 0;
    }

private:
    string extract_key(const string &line) const {
        size_t pos = line.find('=');
        return (pos == string::npos) ? line : line.substr(0, pos);
    }

    istream &in_;
    ostream &out_;
    unordered_set<string> seen_;
    unordered_set<string> reported_;
    vector<string> duplicates_;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ConfigChecker checker(cin, cout);
    return checker.run();
}
