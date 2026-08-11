#include <string>
#include <variant>
#include <iostream>
#include <vector>

//　関節位置イベント
struct JointPosition {
    std::string joint_name;
    double position;
};

// エラーイベント
struct ErrorMessage {
    int error_code;
    std::string message;
};

// 緊急停止イベント
struct EmergencyStop {
    bool active;
};

using RobotEvent = std::variant<
    JointPosition,
    ErrorMessage,
    EmergencyStop
>;

template<class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};

template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

void processEvent(const RobotEvent& event) {
    std::visit(
        Overloaded{
            [](const JointPosition& value) {
                std::cout
                    << "Joint "
                    << value.joint_name
                    << " position: "
                    << value.position
                    << " rad\n";
            },

            [](const ErrorMessage& value) {
                std::cout
                    << "Error "
                    << value.error_code
                    << ": "
                    << value.message
                    << '\n';
            },

            [](const EmergencyStop& value) {
                std::cout
                    << "Emergency stop: "
                    << (value.active ? "ACTIVE" : "INACTIVE")
                    << '\n';
            }
        },
        event
    );
}

int main() {
    std::vector<RobotEvent> events = {
        JointPosition{"shoulder", 1.25},
        JointPosition{"elbow", -0.50},
        ErrorMessage{404, "Motor not found"},
        EmergencyStop{true}
    };

    for (const RobotEvent& event : events) {
        processEvent(event);
    }

    return 0;
}