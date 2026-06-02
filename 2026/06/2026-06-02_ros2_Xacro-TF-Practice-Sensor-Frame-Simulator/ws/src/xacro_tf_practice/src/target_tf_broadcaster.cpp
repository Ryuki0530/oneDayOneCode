#include <chrono>
#include <cmath>
#include <memory>

#include "geometry_msgs/msg/transform_stamped.hpp"
#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/transform_broadcaster.h"

using namespace std::chrono_literals;

class TargetTFBroadcaster : public rclcpp::Node
{
public:
  TargetTFBroadcaster()
  : Node("target_tf_broadcaster")
  {
    // 変更点:
    // launchファイルから target_radius を受け取れるようにする
    target_radius_ = this->declare_parameter<double>("target_radius", 1.0);

    // 変更点:
    // TFを配信するためのbroadcasterを作成する
    tf_broadcaster_ = std::make_unique<tf2_ros::TransformBroadcaster>(*this);

    start_time_ = this->now();

    // 変更点:
    // 0.1秒ごとに target_frame の位置を更新してTF配信する
    timer_ = this->create_wall_timer(
      100ms,
      std::bind(&TargetTFBroadcaster::broadcastTargetFrame, this)
    );

    RCLCPP_INFO(
      this->get_logger(),
      "target_tf_broadcaster started. target_radius=%.2f",
      target_radius_
    );
  }

private:
  void broadcastTargetFrame()
  {
    // パラメータを毎回読み直す
    // ros2 param set で実行中に変更できるようにするため
    target_radius_ = this->get_parameter("target_radius").as_double();

    const rclcpp::Time now = this->now();
    const double t = (now - start_time_).seconds();

    geometry_msgs::msg::TransformStamped transform;

    transform.header.stamp = now;
    transform.header.frame_id = "base_link";
    transform.child_frame_id = "target_frame";

    // 変更点:
    // base_link の周囲を円運動する座標を作る
    transform.transform.translation.x = target_radius_ * std::cos(t);
    transform.transform.translation.y = target_radius_ * std::sin(t);
    transform.transform.translation.z = 0.0;

    // 今回は回転を扱わないので単位クォータニオンにする
    transform.transform.rotation.x = 0.0;
    transform.transform.rotation.y = 0.0;
    transform.transform.rotation.z = 0.0;
    transform.transform.rotation.w = 1.0;

    tf_broadcaster_->sendTransform(transform);
  }

  double target_radius_;
  rclcpp::Time start_time_;
  rclcpp::TimerBase::SharedPtr timer_;
  std::unique_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<TargetTFBroadcaster>());
  rclcpp::shutdown();
  return 0;
}
