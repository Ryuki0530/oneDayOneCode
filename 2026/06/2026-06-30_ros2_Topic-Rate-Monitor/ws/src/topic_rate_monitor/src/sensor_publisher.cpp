#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

using namespace std::chrono_literals;

class SensorPublisher : public rclcpp::Node
{
public:
	SensorPublisher()
	: rclcpp::Node("sensor_publisher")
	{
		publish_period_ms_ = this->declare_parameter<int>("publish_period_ms", 100);

		publisher_ = this->create_publisher<std_msgs::msg::Float32>("/sensor_value", 10);

		timer_ = this->create_wall_timer(
			std::chrono::milliseconds(publish_period_ms_),
			std::bind(&SensorPublisher::onTimer, this));
	}

private:
	void onTimer()
	{
		std_msgs::msg::Float32 msg;
		msg.data = 0.0f;
		publisher_->publish(msg);
	}

	int publish_period_ms_;
	rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr publisher_;
	rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char * argv[])
{
	rclcpp::init(argc, argv);
	rclcpp::spin(std::make_shared<SensorPublisher>());
	rclcpp::shutdown();
	return 0;
}
