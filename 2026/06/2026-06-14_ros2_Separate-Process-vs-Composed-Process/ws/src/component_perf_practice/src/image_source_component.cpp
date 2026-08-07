#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <chrono>
#include <memory>
#include <vector>

using namespace std::chrono_literals;

class ImageSourceComponent : public rclcpp::Node
{
public:
  ImageSourceComponent(const rclcpp::NodeOptions & options = rclcpp::NodeOptions())
  : rclcpp::Node("image_source", options)
  {
    // Publisher設定
    publisher_ = this->create_publisher<sensor_msgs::msg::Image>("/dummy_image", 10);

    // Timer設定: 10ms周期(100Hz)
    timer_ = this->create_wall_timer(
      10ms,
      std::bind(&ImageSourceComponent::publish_image, this)
    );

    RCLCPP_INFO(this->get_logger(), "ImageSourceComponent initialized");
  }

private:
  void publish_image()
  {
    auto message = std::make_shared<sensor_msgs::msg::Image>();

    // 画像情報設定
    message->header.stamp = this->now();
    message->header.frame_id = "camera";
    message->height = 480;
    message->width = 640;
    message->encoding = "mono8";
    message->step = 640;  // width * bytes_per_pixel (1 for mono8)

    // データ領域確保: 640 * 480 bytes
    message->data.resize(640 * 480);
    
    // ダミー画像データ生成 (パターン)
    for (size_t i = 0; i < 640 * 480; ++i) {
      message->data[i] = static_cast<uint8_t>(i % 256);
    }

    // Publish
    publisher_->publish(*message);
  }

  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr publisher_;
  rclcpp::TimerBase::SharedPtr timer_;
};

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(ImageSourceComponent)
