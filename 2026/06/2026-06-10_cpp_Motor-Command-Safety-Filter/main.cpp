#include <iostream>
#include <vector>

using namespace std;

int clampCommand(int v) {
    if (v < -100) {
        return -100;
    } else if (v > 100) {
        return 100;
    } else {
        return v;
    }
}

int main() {
    int n;
    cin >> n;
    
    vector<int> commands(n);
    vector<int> corrected(n);
    int correctionCount = 0;
    
    for (int i = 0; i < n; i++) {
        cin >> commands[i];
        int clamped = clampCommand(commands[i]);
        corrected[i] = clamped;        
        if (commands[i] != clamped) {
            correctionCount++;
        }
    }

    for (int i = 0; i < n; i++) {
        if (i > 0) cout << " ";
        cout << corrected[i];
    }
    cout << "\n";    
    cout << correctionCount << "\n";
    
    return 0;
}
