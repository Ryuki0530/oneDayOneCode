#include <iostream>
#include <memory>
#include <string>
#include <vector>

class ISensor {
public:
	virtual ~ISensor() = default;
	virtual std::string name() const = 0;
	virtual double readValue() const = 0;
};

class TemperatureSensor : public ISensor {
public:
	std::string name() const override { return "temperature"; }
	double readValue() const override { return 36.5; }
};

class SpeedSensor : public ISensor {
public:
	std::string name() const override { return "speed"; }
	double readValue() const override { return 72.0; }
};

class BatterySensor : public ISensor {
public:
	std::string name() const override { return "battery"; }
	double readValue() const override { return 84.0; }
};

void printSensor(const ISensor& sensor) {
	std::cout << sensor.name() << ": " << sensor.readValue() << '\n';
}

int main() {
	std::vector<std::unique_ptr<ISensor>> sensors;
	sensors.emplace_back(std::make_unique<TemperatureSensor>());
	sensors.emplace_back(std::make_unique<SpeedSensor>());
	sensors.emplace_back(std::make_unique<BatterySensor>());

	for (const auto& sensor : sensors) {
		printSensor(*sensor);
	}

	return 0;
}
