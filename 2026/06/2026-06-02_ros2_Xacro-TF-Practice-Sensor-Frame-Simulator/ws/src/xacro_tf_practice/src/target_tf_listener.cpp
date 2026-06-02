#include <chrono>
#include <cmath>
#include <memory>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2/exceptions.h"
#include "tf2/time.h"
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"

using namespace std::chrono_literals;

class TargetTFListener : public rclcpp::Node
{
public:
  TargetTFListener()
  : Node("target_tf_listener")
  {
    // 変更点:
    // TFを蓄積するBufferを作成する
    tf_buffer_ = std::make_unique<tf2_ros::Buffer>(this->get_clock());

    // 変更点:
    // /tf と /tf_static を受信するlistenerを作成する
    tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

    // 変更点:
    // 0.5秒ごとに base_link -> target_frame のTFを取得する
    timer_ = this->create_wall_timer(
      500ms,
      std::bind(&TargetTFListener::lookupTargetFrame, this)
    );

    RCLCPP_INFO(this->get_logger(), "target_tf_listener started.");
  }

private:
  void lookupTargetFrame()
  {
    try {
      geometry_msgs::msg::TransformStamped transform =
        tf_buffer_->lookupTransform(
          "base_link",
          "target_frame",
          tf2::TimePointZero
        );

      const double x = transform.transform.translation.x;
      const double y = transform.transform.translation.y;
      const double distance = std::sqrt(x * x + y * y);

      if (distance >= 1.2) {
        RCLCPP_WARN(
          this->get_logger(),
          "base_link -> target_frame: x=%.2f y=%.2f distance=%.2f",
          x,
          y,
          distance
        );
      } else {
        RCLCPP_INFO(
          this->get_logger(),
          "base_link -> target_frame: x=%.2f y=%.2f distance=%.2f",
          x,
          y,
          distance
        );
      }
    } catch (const tf2::TransformException & ex) {
      RCLCPP_WARN(
        this->get_logger(),
        "Could not transform base_link to target_frame: %s",
        ex.what()
      );
    }
  }

  rclcpp::TimerBase::SharedPtr timer_;
  std::unique_ptr<tf2_ros::Buffer> tf_buffer_;
  std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TargetTFListener>());
  rclcpp::shutdown();
  return 0;
}
