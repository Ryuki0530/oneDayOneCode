#include <bits/stdc++.h>
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

struct Customer{
    std::string name;
    int age;
    double total;
};


enum class SortMode{
    ByTotalThenAgeThenName = 1,
    ByAgeThenTotalThenName = 2,
};

class CustomerManager{
public:
    void reserve(size_t n){
        customers_.reserve(n);
    }
    
    void add(Customer c){
        customers_.push_back(std::move(c));
    }

    void sortBy(SortMode mode){
        if (mode == SortMode::ByTotalThenAgeThenName) {
            // total desc -> age asc -> name asc
            stable_sort(customers_.begin(), customers_.end(),
                        [&](const Customer& a, const Customer& b) {
                            // 浮動小数比較は誤差を考慮（JavaでもBigDecimalで代替するケースに相当）。
                            if (fabs(a.total - b.total) > kEps) return a.total > b.total; // 降順
                            if (a.age != b.age) return a.age < b.age;                      // 昇順
                            return a.name < b.name;                                        // 昇順（辞書順）
                        });
        } else { // SortMode::ByAgeThenTotalThenName
            // age asc -> total desc -> name asc
            stable_sort(customers_.begin(), customers_.end(),
                        [&](const Customer& a, const Customer& b) {
                            if (a.age != b.age) return a.age < b.age;                      // 昇順
                            if (fabs(a.total - b.total) > kEps) return a.total > b.total; // 降順
                            return a.name < b.name;                                        // 昇順
                        });
        }
    }

    void printSortedList(ostream& os) const {
        os << "=== Sorted List ===\n";
        for (const auto& c : customers_) {
            os << c.name << ' ' << c.age << ' ' << c.total << '\n';
        }
        os << '\n';
    }

    // 線形探索
    void searchByAge(int age, ostream& os) const {
        os << "=== Search (age=" << age << ") ===\n";
        bool found = false;
        for (const auto& c : customers_) {
            if (c.age == age) {
                os << c.name << ' ' << c.age << ' ' << c.total << '\n';
                found = true;
            }
        }
        if (!found) os << "not found\n";
    }

private:
    vector<Customer> customers_;
    // 浮動小数誤差用の許容値。
    static constexpr double kEps = 1e-9;
};

int main(){
    //C,C++のストリームを切る。
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N ;
    if (!(cin >> N)) return 1;

    CustomerManager mgr;
    //データ確保用関数(保護のために負数入力を防止)
    mgr.reserve(max(0,N));

    for (int i = 0; i<N; i++){
        Customer c;
        cin >> c.name >> c.age >> c.total;
        mgr.add(std::move(c));
    }

    int modeInt;
    cin >> modeInt;
    int queryAge;
    cin >> queryAge;

    cout << fixed << setprecision(1);

    mgr.sortBy(modeInt == 2 ? SortMode::ByAgeThenTotalThenName
                            : SortMode::ByTotalThenAgeThenName);

    mgr.printSortedList(cout);
    mgr.searchByAge(queryAge, cout);

}