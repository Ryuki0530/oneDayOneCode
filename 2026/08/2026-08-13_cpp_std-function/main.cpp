#include <iostream>
#include <unordered_map>
#include <functional>

class Event {
public:
    using Callback = std::function<void(int)>;

    int subscribe(Callback cb) {
        int id = next_id_++;
        callbacks_.emplace(id, std::move(cb));
        return id;
    }

    bool unsubscribe(int id) {
        return callbacks_.erase(id) > 0;
    }

    void notify(int value) const {
        for (const auto &p : callbacks_) {
            if (p.second) p.second(value);
        }
    }

private:
    std::unordered_map<int, Callback> callbacks_;
    int next_id_ = 1;
};

int main() {
    Event ev;

    int id1 = ev.subscribe([](int v){ 
        std::cout << "Value: " << v << '\n'; 
    });
    int id2 = ev.subscribe([](int v){ 
        v = v * 2;
        std::cout << "Double: " << v << '\n'; 
    });

    ev.notify(10);

    ev.unsubscribe(id1);

    ev.notify(20);

    return 0;
}
