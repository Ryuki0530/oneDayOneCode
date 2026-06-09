#include <chrono>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;

class ManagedSensorNode : public rclcpp_lifecycle::LifecycleNode
{
public:
  ManagedSensorNode()
  : rclcpp_lifecycle::LifecycleNode("managed_sensor_node")
  {
    // 今回の実装ポイント:
    // Lifecycle Nodeでも通常のROS2 parameterと同じように宣言できる
    this->declare_parameter<std::string>("status_message", "sensor is running");

    RCLCPP_INFO(this->get_logger(), "managed_sensor_node created");
  }

private:
  using CallbackReturn =
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

  CallbackReturn on_configure(const rclcpp_lifecycle::State & previous_state) override
  {
    (void)previous_state;

    // 今回の実装ポイント:
    // configure時にpublisherとtimerを作成する
    // ただし、まだactiveではないのでpublishは行わない
    status_message_ = this->get_parameter("status_message").as_string();

    publisher_ = this->create_publisher<std_msgs::msg::String>(
      "/sensor_status",
      10
    );

    timer_ = this->create_wall_timer(
      1s,
      std::bind(&ManagedSensorNode::publish_status, this)
    );

    RCLCPP_INFO(this->get_logger(), "configured");
    RCLCPP_INFO(this->get_logger(), "status_message: %s", status_message_.c_str());

    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override
  {
    (void)previous_state;

    if (!publisher_) {
      RCLCPP_ERROR(this->get_logger(), "publisher is not configured");
      return CallbackReturn::FAILURE;
    }

    // LifecyclePublisherはactivateしないと実際にpublishされない
    publisher_->on_activate();

    RCLCPP_INFO(this->get_logger(), "activated");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override
  {
    (void)previous_state;

    if (publisher_) {
      publisher_->on_deactivate();
    }

    RCLCPP_INFO(this->get_logger(), "deactivated");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_cleanup(const rclcpp_lifecycle::State & previous_state) override
  {
    (void)previous_state;
    // cleanup時にconfigureで作成したリソースを破棄する
    timer_.reset();
    publisher_.reset();

    RCLCPP_INFO(this->get_logger(), "cleaned up");
    return CallbackReturn::SUCCESS;
  }

  CallbackReturn on_shutdown(const rclcpp_lifecycle::State & previous_state) override
  {
    (void)previous_state;

    timer_.reset();
    publisher_.reset();

    RCLCPP_INFO(this->get_logger(), "shutdown");
    return CallbackReturn::SUCCESS;
  }

  void publish_status()
  {
    if (!publisher_) {
      return;
    }

    // timer自体はinactiveでも動くため、active状態かどうかを確認する
    if (!publisher_->is_activated()) {
      return;
    }

    std_msgs::msg::String msg;
    msg.data = status_message_;

    publisher_->publish(msg);

    RCLCPP_INFO(this->get_logger(), "published: %s", msg.data.c_str());
  }

  rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::string status_message_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

  auto node = std::make_shared<ManagedSensorNode>();

  rclcpp::spin(node->get_node_base_interface());

  rclcpp::shutdown();
  return 0;
}