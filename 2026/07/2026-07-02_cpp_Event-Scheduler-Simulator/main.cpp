#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

class Scheduler {
public:
	using Callback = std::function<void()>;

	struct Event {
		long long timeMs{};
		std::string name;
		Callback callback;
		std::size_t order{};
		bool canceled{false};
	};

	void addEvent(long long timeMs, std::string name, Callback callback) {
		events_.push_back(std::make_shared<Event>(Event{timeMs, std::move(name), std::move(callback), nextOrder_++, false}));
		sorted_ = false;
	}

	void cancelEvent(const std::string& name) {
		for (auto& event : events_) {
			if (event && event->name == name) {
				event->canceled = true;
			}
		}
	}

	void run() {
		sortEvents();

		std::size_t index = 0;
		long long currentTime = 0;
		while (index < events_.size()) {
			auto event = events_[index++];
			if (!event || event->canceled) {
				continue;
			}

			if (event->timeMs > currentTime) {
				std::this_thread::sleep_for(std::chrono::milliseconds(event->timeMs - currentTime));
				currentTime = event->timeMs;
			}

			std::cout << event->timeMs << "ms : " << event->name << '\n';
			if (event->callback) {
				event->callback();
			}

			if (!sorted_) {
				sortEvents();
				index = findNextIndexAfter(index, event->order);
			}
		}
	}

private:
	void sortEvents() {
		std::stable_sort(events_.begin(), events_.end(), [](const auto& a, const auto& b) {
			if (a->timeMs != b->timeMs) {
				return a->timeMs < b->timeMs;
			}
			return a->order < b->order;
		});
		sorted_ = true;
	}

	std::size_t findNextIndexAfter(std::size_t startIndex, std::size_t order) const {
		for (std::size_t i = startIndex; i < events_.size(); ++i) {
			if (events_[i] && events_[i]->order > order) {
				return i;
			}
		}
		return events_.size();
	}

	std::vector<std::shared_ptr<Event>> events_;
	std::size_t nextOrder_{0};
	bool sorted_{true};
};

int main() {
	Scheduler scheduler;

	scheduler.addEvent(1000, "START_SENSOR", [] {});
	scheduler.addEvent(500, "LOG", [] {});
	scheduler.addEvent(1500, "STOP_SENSOR", [] {});
	scheduler.addEvent(800, "CHECK_STATUS", [] {});

	scheduler.run();
	return 0;
}
