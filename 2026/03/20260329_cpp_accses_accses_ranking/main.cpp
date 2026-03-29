#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
    string username, pagename;
    map<string, int> pageCount;
    
    while (cin >> username >> pagename) {
        pageCount[pagename]++;
    }
    
    vector<pair<string, int>> pages(pageCount.begin(), pageCount.end());
    
    sort(pages.begin(), pages.end(), [](const auto& a, const auto& b) {
        if (a.second != b.second) {
            return a.second > b.second;
        }
        return a.first < b.first;
    });
    
    for (const auto& p : pages) {
        cout << p.first << " " << p.second << "\n";
    }
    
    return 0;
}