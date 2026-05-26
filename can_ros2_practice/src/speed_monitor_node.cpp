#include <rclcpp/rclcpp.hpp>
#include <can_msgs/msg/frame.hpp>

class SpeedMonitorNode : public rclcpp::Node
{
public:
    SpeedMonitorNode() : Node("speed_monitor_node")
    {
        subscription_ = this->create_subscription<can_msgs::msg::Frame>(
            "/from_can_bus",
            10,
            std::bind(&SpeedMonitorNode::on_can_frame, this, std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "speed_monitor_node started");
    }

private:
    void on_can_frame(const can_msgs::msg::Frame::SharedPtr msg)
    {
        if (msg->id != 0x100) {
            return;
        }

        if (msg->dlc < 1) {
            RCLCPP_WARN(this->get_logger(), "Received CAN ID 0x100, but data is empty");
            return;
        }

        int speed = static_cast<int>(msg->data[0]);

        if (speed >= 60) {
            RCLCPP_WARN(this->get_logger(), "Speed too high: %d km/h", speed);
        } else {
            RCLCPP_INFO(this->get_logger(), "Speed: %d km/h", speed);
        }
    }

    rclcpp::Subscription<can_msgs::msg::Frame>::SharedPtr subscription_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SpeedMonitorNode>());
    rclcpp::shutdown();
    return 0;
}