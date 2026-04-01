#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

bool compare(const pair<string, int>& a, const pair<string, int>& b) {
    if (a.second != b.second) {
        return a.second > b.second;
    }
    return a.first < b.first;
}

int main() {
    int N;
    cin >> N;

    map<string, int> sales;

    for (int i = 0; i < N; ++i) {
        string product;
        int quantity;
        cin >> product >> quantity;
        sales[product] += quantity;
    }

    vector<pair<string, int>> salesVector(sales.begin(), sales.end());
    sort(salesVector.begin(), salesVector.end(), compare);

    for (const auto& entry : salesVector) {
        cout << entry.first << " " << entry.second << endl;
    }

    return 0;
}