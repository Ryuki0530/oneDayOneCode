#include <iostream>
#include <string>
#include <type_traits>
#include <variant>
#include <vector>

int main() {
	using SensorData = std::variant<int, double, std::string>;

	std::vector<SensorData> data{
		80,
		13.8,
		std::string{"READY"},
		100,
		12.5,
		std::string{"ERROR"}
	};

	for (const auto& value : data) {
		std::visit([](const auto& v) {
			using T = std::decay_t<decltype(v)>;

			if constexpr (std::is_same_v<T, int>) {
				std::cout << "Speed : " << v << " km/h\n";
			} else if constexpr (std::is_same_v<T, double>) {
				std::cout << "Voltage : " << v << " V\n";
			} else if constexpr (std::is_same_v<T, std::string>) {
				std::cout << "Status : " << v << '\n';
			}
		}, value);
	}

	return 0;
}
