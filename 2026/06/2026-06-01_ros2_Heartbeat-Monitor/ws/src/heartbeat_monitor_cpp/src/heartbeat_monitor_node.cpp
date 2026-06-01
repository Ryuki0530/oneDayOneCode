#include <chrono>
#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"

using namespace std::chrono_literals;

class HeartbeatMonitorNode : public rclcpp::Node
{
public:
    HeartbeatMonitorNode()
    : Node("heartbeat_monitor_node"),
      received_once_(false),
      timeout_sec_(2.0)
    {
        // 変更点: タイムアウト秒数をROS2パラメータとして変更可能にする
        this->declare_parameter<double>("timeout_sec", 2.0);
        timeout_sec_ = this->get_parameter("timeout_sec").as_double();

        // 変更点: /heartbeat を購読するSubscriber
        heartbeat_sub_ = this->create_subscription<std_msgs::msg::Bool>(
            "/heartbeat",
            10,
            std::bind(&HeartbeatMonitorNode::heartbeatCallback, this, std::placeholders::_1)
        );

        // 変更点: 1秒ごとにHeartbeatの受信状況を確認するTimer
        timer_ = this->create_wall_timer(
            1s,
            std::bind(&HeartbeatMonitorNode::checkHeartbeatTimeout, this)
        );

        RCLCPP_INFO(
            this->get_logger(),
            "Heartbeat monitor started. timeout_sec = %.2f",
            timeout_sec_
        );
    }

private:
    void heartbeatCallback(const std_msgs::msg::Bool::SharedPtr msg)
    {
        // 変更点: Heartbeatを受信した時刻を記録
        last_heartbeat_time_ = this->now();
        received_once_ = true;

        if (msg->data) {
            RCLCPP_INFO(this->get_logger(), "Heartbeat received: true");
        } else {
            RCLCPP_WARN(this->get_logger(), "Heartbeat received, but status is false");
        }
    }

    void checkHeartbeatTimeout()
    {
        // 変更点: まだ一度もHeartbeatを受信していない場合
        if (!received_once_) {
            RCLCPP_WARN(this->get_logger(), "Heartbeat has not been received yet");
            return;
        }

        // 変更点: 最後に受信してからの経過時間を計算
        const double elapsed_sec = (this->now() - last_heartbeat_time_).seconds();

        if (elapsed_sec >= timeout_sec_) {
            RCLCPP_WARN(
                this->get_logger(),
                "Heartbeat timeout. elapsed = %.2f sec",
                elapsed_sec
            );
        } else {
            RCLCPP_INFO(
                this->get_logger(),
                "Heartbeat OK. elapsed = %.2f sec",
                elapsed_sec
            );
        }
    }

    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr heartbeat_sub_;
    rclcpp::TimerBase::SharedPtr timer_;

    rclcpp::Time last_heartbeat_time_;

    bool received_once_;
    double timeout_sec_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    auto node = std::make_shared<HeartbeatMonitorNode>();
    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}