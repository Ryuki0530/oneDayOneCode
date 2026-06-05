import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32


class SpeedMonitorNode(Node):
    def __init__(self):
        super().__init__('speed_monitor_node')
        self.declare_parameter('threshold', 60.0)
        self.threshold = self.get_parameter('threshold').value

        self.get_logger().info(
            f'Speed monitor started. threshold = {self.threshold} km/h'
        )

        self.subscription = self.create_subscription(
            Float32,
            '/vehicle/speed',
            self.speed_callback,
            10
        )

    def speed_callback(self, msg):
        speed = msg.data

        if speed >= self.threshold:
            self.get_logger().warn(
                f'Speed too high: {speed:.1f} km/h'
            )
        else:
            self.get_logger().info(
                f'Speed OK: {speed:.1f} km/h'
            )


def main(args=None):
    rclpy.init(args=args)

    node = SpeedMonitorNode()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()