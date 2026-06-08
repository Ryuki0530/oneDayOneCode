#include <rclcpp/rclcpp.hpp>

#define SPEED_LIMIT_THRESHOLD 60

class SpeedCheckerNode : public rclcpp::Node
{
public:
  SpeedCheckerNode():Node("speed_checker_node")
  {
    RCLCPP_INFO(this->get_logger(), "Speed Checker Node has been started.");
    this -> declare_parameter<int>("speed_limit", SPEED_LIMIT_THRESHOLD);
    const int speed_limit = this -> get_parameter("speed_limit").as_int();

    /*
    speed_limit が 80 以上:
    [WARN] speed_limit=80 high speed threshold
    speed_limit が 80 未満:
    [INFO] speed_limit=60 normal speed threshold
    */

    if (speed_limit < 0) {
      RCLCPP_ERROR(this->get_logger(), "Invalid speed_limit: %d. It must be non-negative.", speed_limit);
    } else if (speed_limit >= 80) {
      RCLCPP_WARN(this->get_logger(), "speed_limit=%d high speed threshold", speed_limit);
    } else if (speed_limit >= 60) {
      RCLCPP_INFO(this->get_logger(), "speed_limit=%d normal speed threshold", speed_limit);
    } else {
      RCLCPP_ERROR(this->get_logger(), "Invalid speed_limit: %d. It must be a positive integer.", speed_limit);
    }
  }
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SpeedCheckerNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}