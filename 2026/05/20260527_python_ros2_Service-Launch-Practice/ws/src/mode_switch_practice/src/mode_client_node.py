import sys
import rclpy
from rclpy.node import Node
from std_srvs.srv import SetBool

class ModeClientNode(Node):
    def __init__(self, auto: bool):
        super().__init__('mode_client_node')
        self.client = self.create_client(SetBool, '/set_auto_mode')
        if not self.client.wait_for_service(SetBool, timeout_sec=5.0):
            self.get_logger().error("Service '/set_auto_mode' not available.")
            raise RuntimeError("Service '/set_auto_mode' not available.")

        request = SetBool.Request()
        request.data = bool(auto)

        future = self.client.call_async(request) 
        rclpy.spin_until_future_complete(self, future)
        if future.result() is not None:
            self.get_logger().info(f"Service call successful: {future.result().success}")
        else:
            self.get_logger().error("Service call failed")

def main(args=None):
    args = sys.argv if args is None else args
    if len(args) != 2:
        print("Usage: mode_client_node.py [auto|manual]")
        return 2
    
    arg = args[1].lower()
    
    if arg not in ['auto', 'manual']:
        print("Invalid argument. Use 'auto' or 'manual'.")
        return 2
    
    if arg == 'auto':
        auto = True
    if arg == 'manual': 
        auto = False
    else:
        print("Invalid argument. Use 'auto' or 'manual'.")
        return 2
    
    rclpy.init()
    try:
        node = ModeClientNode(auto)
    except RuntimeError:
        rclpy.shutdown()
        return 1
    
    node.destroy_node()
    rclpy.shutdown()
    return 0

if __name__ == '__main__':
    sys.exit(main())