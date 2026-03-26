#include <iostream>
#include <unordered_map>
#include <sstream>
using namespace std;

int main() {
    unordered_map<int, int> count;
    string line;
    
    getline(cin, line);
    stringstream ss(line);
    
    int x;
    while (ss >> x) {
        count[x]++;
    }
    
    int maxCount = 0;
    int result = 0;
    
    for (auto& p : count) {
        int value = p.first;
        int freq = p.second;
        
        if (freq > maxCount || (freq == maxCount && value < result)) {
            maxCount = freq;
            result = value;
        }
    }
    
    cout << result << endl;
    
    return 0;
}
