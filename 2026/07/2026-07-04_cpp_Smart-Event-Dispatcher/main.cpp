#include <iostream>
#include <string>
#include <functional>
#include <unordered_map>
#include <vector>

class EventDispatcher {
private:
    std::unordered_map<std::string, std::vector<std::function<void()>>> callbacks;

public:
    void subscribe(const std::string& event_name, std::function<void()> callback) {
        callbacks[event_name].push_back(callback);
    }

    void publish(const std::string& event_name) {
        auto it = callbacks.find(event_name);
        if (it != callbacks.end()) {
            for (auto& callback : it->second) {
                callback();
            }
        }
    }
};

int main() {
    EventDispatcher dispatcher;

    dispatcher.subscribe("start", [](){
        std::cout << "System Start\n";
    });

    dispatcher.subscribe("start", [](){
        std::cout << "Initialize Sensor\n";
    });

    dispatcher.subscribe("stop", [](){
        std::cout << "System Stop\n";
    });

    dispatcher.publish("start");

    dispatcher.publish("stop");

    return 0;
}
