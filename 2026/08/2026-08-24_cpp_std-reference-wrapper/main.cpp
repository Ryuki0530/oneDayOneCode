#include <functional>
#include <iostream>
#include <string>
#include <vector>

class Device
{
public:
	Device(std::string name, bool enabled)
		: name_(std::move(name)), enabled_(enabled)
	{
	}

	void enable()
	{
		enabled_ = true;
	}

	void disable()
	{
		enabled_ = false;
	}

	bool isEnabled() const
	{
		return enabled_;
	}

	const std::string& getName() const
	{
		return name_;
	}

private:
	std::string name_;
	bool enabled_;
};

int main()
{
	Device camera("Camera", true);
	Device lidar("LiDAR", false);
	Device motor("Motor", true);

	std::vector<std::reference_wrapper<Device>> devices;
	devices.push_back(std::ref(camera));
	devices.push_back(std::ref(lidar));
	devices.push_back(std::ref(motor));

	for (Device& device : devices) {
		if (device.isEnabled()) {
			device.disable();
		} else {
			device.enable();
		}
	}

	for (const Device& device : devices) {
		std::cout << device.getName() << " : "
				  << (device.isEnabled() ? "enabled" : "disabled")
				  << '\n';
	}

	return 0;
}
