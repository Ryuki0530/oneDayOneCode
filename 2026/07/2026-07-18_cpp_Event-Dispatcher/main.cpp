#define DEBUG 1
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <string>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <sstream>

enum class EventType{
    CONNECT,
    DISCONNECT,
    ERROR
};

struct EventTypeHash {
    std::size_t operator()(const EventType& type) const {
        return static_cast<std::size_t>(type);
    }
};

class EventDispatcher {
public:
    void dispatchEvent(const std::string& event) {
        // Implementation for dispatching the event
    }

    void subscribe(EventType type,std::function<void(const std::string&)> callback){
        callbacks_[type].push_back(std::move(callback));
        if(DEBUG){
            std::cout << "Subscribed to event of type: " << static_cast<int>(type) << std::endl;
        }
    }

    void publish(EventType type,const std::string& message){
        if(DEBUG){
            std::cout << "Publishing event of type: " << static_cast<int>(type) << " with message: " << message << std::endl;
        }
        auto it = callbacks_.find(type);
        if (it  == callbacks_.end()) {
            return;
        }
        for (const auto& callback : it->second) {
            callback(message);
        }
    }

private:
    std::unordered_map<
        EventType,
        std::vector<std::function<void(const std::string&)>>,
        EventTypeHash
    > callbacks_;
};

int main (void){
    std::vector<std::string> connected_clients;
    int num_of_connected_clients = 0;
    EventDispatcher dispatcher;
    dispatcher.subscribe(EventType::CONNECT, [&num_of_connected_clients, &connected_clients](const std::string& message){
        std::cout << "Connect event received with message: " << message << std::endl;
        num_of_connected_clients++;
        connected_clients.push_back(message);
    });
    dispatcher.subscribe(EventType::DISCONNECT, [&num_of_connected_clients, &connected_clients](const std::string& message){
        std::cout << "Disconnect event received with message: " << message << std::endl;
        num_of_connected_clients--;
        connected_clients.erase(std::remove(connected_clients.begin(), connected_clients.end(), message), connected_clients.end());
    });
    dispatcher.subscribe(EventType::ERROR, [&num_of_connected_clients, &connected_clients](const std::string& message){
        std::cerr << "Error event received with message: " << message << std::endl;
        std::cout << "Clients currently connected \n ------------------ ";
        for(const auto& client : connected_clients){
            std::cout << "\n" << client;
        } 
        std::cout << "\n ------------------ \nNumber of connected clients: " << num_of_connected_clients << std::endl;
        num_of_connected_clients = 0;
        exit(1);
    });

    std::cout << "Event Dispatcher Example" << std::endl;
    while(true){
        std::string raw_command;
        std::cout << "> ";
        std::getline(std::cin, raw_command);
        std::istringstream iss(raw_command);
        std::vector<std::string> tokens;
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }

        if(tokens.empty()){
            continue;
        }

        const std::string& command = tokens[0];
        const std::string& target = tokens.size() > 1 ? tokens[1] : "";
        if(command == ""){
            break;
        } else if(command == "CONNECTED"){
            dispatcher.publish(EventType::CONNECT, target);
        } else if(command == "DISCONNECT"){
            dispatcher.publish(EventType::DISCONNECT, target);
        } else if(command == "ERROR"){
            dispatcher.publish(EventType::ERROR, target);
        } else {
            std::cout << "Unknown event type. Please try again." << std::endl;
        }
    }
    
    
    
    return 0;
}

