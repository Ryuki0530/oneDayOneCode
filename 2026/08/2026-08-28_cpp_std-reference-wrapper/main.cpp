 #include <functional>
#include <iostream>
#include <string>
#include <vector>

class Sensor {
public:
	Sensor(std::string name, double value)
		: name_(std::move(name)), value_(value) {}

	void setValue(double value) {
		value_ = value;
	}

	double getValue() const {
		return value_;
	}

	void print() const {
		std::cout << name_ << ": " << value_ << '\n';
	}

private:
	std::string name_;
	double value_;
};

class SensorManager {
public:
	void add(Sensor& sensor) {
		sensors_.push_back(std::ref(sensor));
	}

	void printAll() const {
		for (const Sensor& sensor : sensors_) {
			sensor.print();
		}
	}

private:
	std::vector<std::reference_wrapper<Sensor>> sensors_;
};

int main() {
	Sensor temperature("temperature", 25.3);
	Sensor pressure("pressure", 101.3);
	Sensor humidity("humidity", 45.0);

	SensorManager manager;
	manager.add(temperature);
	manager.add(pressure);
	manager.add(humidity);

	temperature.setValue(30.0);
	manager.printAll();
}
