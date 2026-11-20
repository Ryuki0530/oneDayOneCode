#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

class EventDispatcher {
public:
	int subscribe(const std::string& event,
				  std::function<void(int)> callback) {
		const int id = next_id_++;
		listeners_.emplace(id, Listener{event, std::move(callback)});
		event_listeners_[event].push_back(id);
		return id;
	}

	bool unsubscribe(int id) {
		const auto listener = listeners_.find(id);
		if (listener == listeners_.end()) {
			return false;
		}

		const auto event = event_listeners_.find(listener->second.event);
		if (event != event_listeners_.end()) {
			auto& ids = event->second;
			for (auto it = ids.begin(); it != ids.end(); ++it) {
				if (*it == id) {
					ids.erase(it);
					break;
				}
			}
			if (ids.empty()) {
				event_listeners_.erase(event);
			}
		}

		listeners_.erase(listener);
		return true;
	}

	void emit(const std::string& event, int value) {
		const auto event_it = event_listeners_.find(event);
		if (event_it == event_listeners_.end()) {
			return;
		}

		// Take a snapshot so subscriptions made during emit run next time.
		const std::vector<int> ids = event_it->second;
		for (const int id : ids) {
			const auto listener = listeners_.find(id);
			if (listener != listeners_.end()) {
				listener->second.callback(value);
			}
		}
	}

	std::size_t listener_count(const std::string& event) const {
		const auto event_it = event_listeners_.find(event);
		return event_it == event_listeners_.end() ? 0 : event_it->second.size();
	}

private:
	struct Listener {
		std::string event;
		std::function<void(int)> callback;
	};

	int next_id_ = 1;
	std::unordered_map<int, Listener> listeners_;
	std::unordered_map<std::string, std::vector<int>> event_listeners_;
};


int main() {
    EventDispatcher dispatcher;

    int id1 = dispatcher.subscribe("temperature", [](int value) {
        std::cout << "Sensor A: " << value << std::endl;
    });

    int id2 = dispatcher.subscribe("temperature", [](int value) {
        std::cout << "Sensor B: " << value * 2 << std::endl;
    });

    dispatcher.emit("temperature", 25);

    dispatcher.unsubscribe(id1);

    dispatcher.emit("temperature", 30);

    std::cout << dispatcher.listener_count("temperature")
            << std::endl;
}