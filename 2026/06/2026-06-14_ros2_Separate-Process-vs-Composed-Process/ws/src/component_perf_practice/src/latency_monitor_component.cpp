#include <algorithm>
#include <chrono>
#include <cstddef>
#include <limits>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rclcpp_components/register_node_macro.hpp"
#include "sensor_msgs/msg/image.hpp"

class LatencyMonitor : public rclcpp::Node
{
public:
	explicit LatencyMonitor(const rclcpp::NodeOptions & options)
	: rclcpp::Node("latency_monitor", options),
		count_(0),
		sum_ms_(0.0),
		min_ms_(std::numeric_limits<double>::max()),
		max_ms_(std::numeric_limits<double>::lowest())
	{
		using std::placeholders::_1;

		sub_ = this->create_subscription<sensor_msgs::msg::Image>(
			"/dummy_image",
			rclcpp::SensorDataQoS(),
			std::bind(&LatencyMonitor::on_image, this, _1));

		timer_ = this->create_wall_timer(
			std::chrono::seconds(1),
			std::bind(&LatencyMonitor::on_timer, this));
	}

private:
	void on_image(const sensor_msgs::msg::Image::SharedPtr msg)
	{
		const rclcpp::Time stamp(msg->header.stamp);
		const double latency_ms = (this->now() - stamp).seconds() * 1000.0;

		++count_;
		sum_ms_ += latency_ms;
		min_ms_ = std::min(min_ms_, latency_ms);
		max_ms_ = std::max(max_ms_, latency_ms);
	}

	void on_timer()
	{
		if (count_ == 0) {
			RCLCPP_INFO(this->get_logger(), "count=0 avg_ms=0.000 min_ms=0.000 max_ms=0.000");
			return;
		}

		const double avg_ms = sum_ms_ / static_cast<double>(count_);
		RCLCPP_INFO(
			this->get_logger(),
			"count=%zu avg_ms=%.3f min_ms=%.3f max_ms=%.3f",
			count_, avg_ms, min_ms_, max_ms_);

		count_ = 0;
		sum_ms_ = 0.0;
		min_ms_ = std::numeric_limits<double>::max();
		max_ms_ = std::numeric_limits<double>::lowest();
	}

	rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
	rclcpp::TimerBase::SharedPtr timer_;

	std::size_t count_;
	double sum_ms_;
	double min_ms_;
	double max_ms_;
};

RCLCPP_COMPONENTS_REGISTER_NODE(LatencyMonitor)

