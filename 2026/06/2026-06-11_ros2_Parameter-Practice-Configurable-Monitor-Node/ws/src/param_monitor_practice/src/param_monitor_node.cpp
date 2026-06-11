#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rcl_interfaces/msg/parameter_descriptor.hpp"

using namespace std::chrono_literals;

class ParamMonitor : public rclcpp::Node
{
public:
  ParamMonitor()
  : Node("param_monitor")
  {
    // robot_name parameter
    auto robot_name_desc = rcl_interfaces::msg::ParameterDescriptor();
    robot_name_desc.description = "Name of the robot. This parameter can be changed while running.";
    robot_name_desc.read_only = false;
    robot_name_desc.dynamic_typing = false;

    // alert_level parameter
    auto alert_level_desc = rcl_interfaces::msg::ParameterDescriptor();
    alert_level_desc.description = "Alert level. This parameter is read-only.";
    alert_level_desc.read_only = true;
    alert_level_desc.dynamic_typing = false;

    // message parameter
    auto message_desc = rcl_interfaces::msg::ParameterDescriptor();
    message_desc.description = "Message text. This parameter allows dynamic typing and deletion.";
    message_desc.read_only = false;
    message_desc.dynamic_typing = true;

    // Parameter declaration
    this->declare_parameter<std::string>("robot_name", "robot_01", robot_name_desc);
    this->declare_parameter<int64_t>("alert_level", 3, alert_level_desc);
    this->declare_parameter<std::string>("message", "hello", message_desc);

    timer_ = this->create_wall_timer(
      1000ms,
      std::bind(&ParamMonitor::timer_callback, this)
    );
  }

private:
  void timer_callback()
  {
    const std::string robot_name = this->get_parameter("robot_name").as_string();
    const int64_t alert_level = this->get_parameter("alert_level").as_int();

    RCLCPP_INFO(this->get_logger(), "robot_name : %s", robot_name.c_str());
    RCLCPP_INFO(this->get_logger(), "alert_level: %lld", static_cast<long long>(alert_level));

    if (this->has_parameter("message")) {
      const auto message_param = this->get_parameter("message");
      RCLCPP_INFO(this->get_logger(), "message    : %s", message_param.value_to_string().c_str());
    } else {
      RCLCPP_WARN(this->get_logger(), "message parameter is not declared");
    }

    RCLCPP_INFO(this->get_logger(), "-----------------------------");
  }

  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ParamMonitor>());
  rclcpp::shutdown();
  return 0;
}
