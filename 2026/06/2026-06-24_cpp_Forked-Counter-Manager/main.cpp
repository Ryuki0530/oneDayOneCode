#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

class Counter {
private:
    int count;

public:
    Counter(int initial = 0) : count(initial) {}

    ~Counter() {
        cout << "Destroy PID=" << getpid() << endl;
    }

    void increment() {
        count++;
    }

    int getCount() {
        return count;
    }

    void print() {
        cout << "PID=" << getpid() << " Count=" << count << endl;
    }
};

int main() {
    Counter counter(0);

    pid_t pid = fork();

    if (pid == 0) {
        // Child process
        counter.increment();
        counter.increment();
        counter.increment();
        counter.print();
    } else {
        // Parent process
        counter.increment();
        counter.increment();
        counter.increment();
        counter.increment();
        counter.increment();
        wait(NULL);
        counter.print();
    }

    return 0;
}
