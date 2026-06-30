#include <chrono>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"

class RateMonitorNode : public rclcpp::Node
{
public:
	RateMonitorNode()
	: Node("rate_monitor"), has_prev_time_(false)
	{
		sub_ = this->create_subscription<std_msgs::msg::Float32>(
			"/sensor_value",
			rclcpp::QoS(10),
			std::bind(&RateMonitorNode::on_message, this, std::placeholders::_1));

		RCLCPP_INFO(this->get_logger(), "Subscribed to /sensor_value");
	}

private:
	void on_message(const std_msgs::msg::Float32::SharedPtr /*msg*/)
	{
		const rclcpp::Time now = this->now();

		if (!has_prev_time_) {
			prev_time_ = now;
			has_prev_time_ = true;
			RCLCPP_INFO(this->get_logger(), "First message received.");
			return;
		}

		const double period_sec = (now - prev_time_).seconds();
		prev_time_ = now;

		if (period_sec <= 0.0) {
			RCLCPP_WARN(this->get_logger(), "Non-positive period detected: %.9f s", period_sec);
			return;
		}

		const double hz = 1.0 / period_sec;
		RCLCPP_INFO(this->get_logger(), "period: %.6f s, rate: %.3f Hz", period_sec, hz);
	}

	rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr sub_;
	rclcpp::Time prev_time_;
	bool has_prev_time_;
};

int main(int argc, char * argv[])
{
	rclcpp::init(argc, argv);
	rclcpp::spin(std::make_shared<RateMonitorNode>());
	rclcpp::shutdown();
	return 0;
}
