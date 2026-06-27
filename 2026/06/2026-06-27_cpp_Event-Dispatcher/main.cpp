#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

class EventDispatcher {
private:
    std::unordered_map<std::string, std::function<void(const std::string&)>> handlers;

public:
    void registerEvent(const std::string& eventName, std::function<void(const std::string&)> handler) {
        handlers[eventName] = handler;
    }

    void dispatch(const std::string& eventLine) {
        size_t spacePos = eventLine.find(' ');
        std::string eventName = (spacePos != std::string::npos) ? eventLine.substr(0, spacePos) : eventLine;
        std::string eventData = (spacePos != std::string::npos) ? eventLine.substr(spacePos + 1) : "";

        auto it = handlers.find(eventName);
        if (it != handlers.end()) {
            it->second(eventData);
        } else {
            std::cout << "Unknown event: " << eventName << std::endl;
        }
    }
};

int main() {
    EventDispatcher dispatcher;

    dispatcher.registerEvent("START", [](const std::string& data) {
        std::cout << "System started" << std::endl;
    });

    dispatcher.registerEvent("STOP", [](const std::string& data) {
        std::cout << "System stopped" << std::endl;
    });

    dispatcher.registerEvent("ERROR", [](const std::string& data) {
        if (data.empty()) {
            std::cout << "Error occurred" << std::endl;
        } else {
            std::cout << "Error occurred: " << data << std::endl;
        }
    });

    std::string input;
    while (std::getline(std::cin, input)) {
        if (!input.empty()) {
            dispatcher.dispatch(input);
        }
    }

    return 0;
}
