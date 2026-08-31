#include <array>
#include <cstdint>
#include <iomanip>
#include <iostream>

struct VehicleStatus {
    double speed_kmh;
    int motor_temperature;
};

std::array<std::uint8_t, 8> encode(const VehicleStatus& status) {
    std::array<std::uint8_t, 8> data{};

    const auto speed_value = static_cast<std::uint16_t>(status.speed_kmh * 10.0 + 0.5);
    const std::uint8_t speed_high = static_cast<std::uint8_t>((speed_value >> 8) & 0xFFu);
    const std::uint8_t speed_low  = static_cast<std::uint8_t>(speed_value & 0xFFu);
    const std::uint8_t temperature = static_cast<std::uint8_t>(status.motor_temperature + 40);

    data[0] = speed_high;
    data[1] = speed_low;
    data[2] = temperature;

    return data;
}

VehicleStatus decode(const std::array<std::uint8_t, 8>& data) {
    const std::uint16_t speed_value =
        (static_cast<std::uint16_t>(data[0]) << 8) |
        static_cast<std::uint16_t>(data[1]);

    const int temperature = static_cast<int>(data[2]) - 40;

    return VehicleStatus{speed_value / 10.0, temperature};
}

int main() {
    const VehicleStatus original{123.4, 25};

    const auto encoded = encode(original);

    std::cout << "Encoded data: ";
    for (std::uint8_t value : encoded) {
        std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0')
                  << static_cast<int>(value) << ' ';
    }
    std::cout << std::dec << std::setfill(' ') << '\n';

    const auto decoded = decode(encoded);

    std::cout << "Speed: " << std::fixed << std::setprecision(1) << decoded.speed_kmh
              << " km/h\n";
    std::cout << "Temperature: " << decoded.motor_temperature << " C\n";

    return 0;
}
