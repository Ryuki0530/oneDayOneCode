import rclpy
from rclpy.node import Node
from std_srvs.srv import SetBool

class ModeManagerNode(Node):
    def __init__(self):
        super().__init__('mode_manager_node')
        self.auto_mode = False

        self.srv = self.create_service(
            SetBool,
            '/set_auto_mode',
            self.set_auto_mode_callback
        )

        self.timer = self.create_timer(2.0, self.timer_callback)

        self.get_logger().info("Node 'mode_manager_node' has been started.")


    def set_auto_mode_callback(self, request, response):
        self.auto_mode = request.data
        self.get_logger().info(f"Auto mode set to: {self.auto_mode}")
        response.success = True
        return response

    def timer_callback(self):
        mode = "AUTO" if self.auto_mode else "MANUAL"
        self.get_logger().info(f"Current mode: {mode}")


def main(args=None):
    rclpy.init(args=args)
    node = ModeManagerNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()