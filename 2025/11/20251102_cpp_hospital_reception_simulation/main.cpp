#include <bits/stdc++.h>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

struct Patient {
    string name;
    int age;
    int urgency;
    int order; // 受付順
};

struct Compare {
    bool operator()(const Patient& a, const Patient& b) const {
        if (a.urgency != b.urgency) return a.urgency < b.urgency; // 緊急度高い方が優先
        return a.order > b.order; // 同じ緊急度なら先に来た方が優先
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
#ifdef _WIN32
    // コンソールの入出力を UTF-8 に設定
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    int N;
    if (!(cin >> N)) return 0;

    priority_queue<Patient, vector<Patient>, Compare> pq;
    for (int i = 0; i < N; ++i) {
        Patient p;
        cin >> p.name >> p.age >> p.urgency;
        p.order = i;
        pq.push(p);
    }

    cout << "診察順:\n\n";
    while (!pq.empty()) {
        const auto p = pq.top();
        pq.pop();
        cout << p.name << "(" << p.age << ") - 緊急度: " << p.urgency << "\n\n";
    }

    return 0;
}