// 使用するメッセージ仕様
// # RobotStatus.msg
// # ロボットの状態を表すメッセージ

// string robot_name
// int32 battery_percent
// float32 temperature
// bool is_emergency

// int32 NORMAL = 0
// int32 WARNING = 1
// int32 ERROR = 2
// int32 status_level

#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/msg/robot_status.hpp"

using namespace std::chrono_literals;

class StatusPublisher : public rclcpp::Node
{
public:
    StatusPublisher()
        : Node("status_publisher"),
          battery_percent_(100)
    {
        publisher_ = this->create_publisher<robot_interfaces::msg::RobotStatus>(
            "/robot/status",
            10
        );

        timer_ = this->create_wall_timer(
            1s,
            std::bind(&StatusPublisher::publish_status, this)
        );

        RCLCPP_INFO(this->get_logger(), "status_publisher node started.");
    }

private:
    void publish_status()
    {
        robot_interfaces::msg::RobotStatus msg;

        msg.robot_name = "so101_arm";
        msg.battery_percent = battery_percent_;
        msg.temperature = 35.0F + static_cast<float>(100 - battery_percent_) * 0.2F;
        msg.is_emergency = battery_percent_ < 10;

        if (battery_percent_ >= 50) {
            msg.status_level = robot_interfaces::msg::RobotStatus::NORMAL;
        } else if (battery_percent_ >= 20) {
            msg.status_level = robot_interfaces::msg::RobotStatus::WARNING;
        } else {
            msg.status_level = robot_interfaces::msg::RobotStatus::ERROR;
        }

        publisher_->publish(msg);

        RCLCPP_INFO(
            this->get_logger(),
            "Published: name=%s, battery=%d%%, temp=%.1f, emergency=%s, level=%d",
            msg.robot_name.c_str(),
            msg.battery_percent,
            msg.temperature,
            msg.is_emergency ? "true" : "false",
            msg.status_level
        );

        battery_percent_--;

        if (battery_percent_ < 0) {
            battery_percent_ = 100;
        }
    }

    rclcpp::Publisher<robot_interfaces::msg::RobotStatus>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    int battery_percent_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<StatusPublisher>();
    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}