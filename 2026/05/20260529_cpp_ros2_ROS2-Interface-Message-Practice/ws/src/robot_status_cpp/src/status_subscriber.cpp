#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/msg/robot_status.hpp"

class StatusSubscriber : public rclcpp::Node
{
public:
    StatusSubscriber()
        : Node("status_subscriber")
    {
        subscription_ = this->create_subscription<robot_interfaces::msg::RobotStatus>(
            "/robot/status",
            10,
            std::bind(&StatusSubscriber::status_callback, this, std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "status_subscriber node started.");
    }

private:
    std::string status_level_to_string(int32_t status_level)
    {
        if (status_level == robot_interfaces::msg::RobotStatus::NORMAL) {
            return "NORMAL";
        }

        if (status_level == robot_interfaces::msg::RobotStatus::WARNING) {
            return "WARNING";
        }

        if (status_level == robot_interfaces::msg::RobotStatus::ERROR) {
            return "ERROR";
        }

        return "UNKNOWN";
    }

    void status_callback(const robot_interfaces::msg::RobotStatus::SharedPtr msg)
    {
        std::string level_text = status_level_to_string(msg->status_level);

        if (msg->is_emergency ||
            msg->status_level == robot_interfaces::msg::RobotStatus::ERROR) {
            RCLCPP_ERROR(
                this->get_logger(),
                "[%s] battery=%d%%, temp=%.1f, emergency=%s, level=%s",
                msg->robot_name.c_str(),
                msg->battery_percent,
                msg->temperature,
                msg->is_emergency ? "true" : "false",
                level_text.c_str()
            );
            return;
        }

        if (msg->status_level == robot_interfaces::msg::RobotStatus::WARNING) {
            RCLCPP_WARN(
                this->get_logger(),
                "[%s] battery=%d%%, temp=%.1f, emergency=%s, level=%s",
                msg->robot_name.c_str(),
                msg->battery_percent,
                msg->temperature,
                msg->is_emergency ? "true" : "false",
                level_text.c_str()
            );
            return;
        }

        RCLCPP_INFO(
            this->get_logger(),
            "[%s] battery=%d%%, temp=%.1f, emergency=%s, level=%s",
            msg->robot_name.c_str(),
            msg->battery_percent,
            msg->temperature,
            msg->is_emergency ? "true" : "false",
            level_text.c_str()
        );
    }

    rclcpp::Subscription<robot_interfaces::msg::RobotStatus>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<StatusSubscriber>();
    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}