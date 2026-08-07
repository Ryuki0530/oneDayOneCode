#include <iostream>
#include <utility>

class State {
private:
    int value;

public:
    State(int initial_value) : value(initial_value) {}

    int update(int new_value) {
        return std::exchange(value, new_value);
    }

    int get() const {
        return value;
    }
};

int main() {
    State state(10);

    std::cout << state.update(20) << '\n';
    std::cout << state.get() << '\n';

    return 0;
}