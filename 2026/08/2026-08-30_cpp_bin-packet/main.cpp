#include <cstdint>
#include <iostream>
#include <optional>
#include <vector>

struct Packet {
    std::uint8_t messageId;
    std::uint16_t sensorValue;
    std::uint8_t status;
};

std::optional<Packet> parsePacket(const std::vector<std::uint8_t>& data) {
    if (data.size() != 5) {
        return std::nullopt;
    }

    const std::uint8_t messageId = data[0];
    const std::uint16_t sensorValue =
        static_cast<std::uint16_t>(static_cast<std::uint16_t>(data[1]) << 8) |
        static_cast<std::uint16_t>(data[2]);
    const std::uint8_t status = data[3];
    const std::uint8_t checksum = data[4];

    const std::uint8_t expectedChecksum =
        static_cast<std::uint8_t>((data[0] + data[1] + data[2] + data[3]) & 0xFFu);

    if (checksum != expectedChecksum) {
        return std::nullopt;
    }

    return Packet{messageId, sensorValue, status};
}

int main() {
    std::vector<std::uint8_t> validData{0x10, 0x12, 0x34, 0x01, 0x57};
    std::vector<std::uint8_t> invalidData{0x10, 0x12, 0x34, 0x01, 0x00};

    const auto validPacket = parsePacket(validData);
    if (validPacket.has_value()) {
        std::cout << "Message ID: " << static_cast<int>(validPacket->messageId) << '\n';
        std::cout << "Sensor value: " << validPacket->sensorValue << '\n';
        std::cout << "Status: " << static_cast<int>(validPacket->status) << '\n';
    } else {
        std::cout << "Invalid packet\n";
    }

    const auto invalidPacket = parsePacket(invalidData);
    if (invalidPacket.has_value()) {
        std::cout << "Message ID: " << static_cast<int>(invalidPacket->messageId) << '\n';
        std::cout << "Sensor value: " << invalidPacket->sensorValue << '\n';
        std::cout << "Status: " << static_cast<int>(invalidPacket->status) << '\n';
    } else {
        std::cout << "Invalid packet\n";
    }

    return 0;
}
