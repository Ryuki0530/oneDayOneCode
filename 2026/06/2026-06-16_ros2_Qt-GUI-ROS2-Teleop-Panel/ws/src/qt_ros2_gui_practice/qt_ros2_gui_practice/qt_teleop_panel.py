#!/usr/bin/env python3

# 変更点:
# Qt GUIとROS2ノードを同じPythonプロセス内で動かす実装。
# Qt側のイベントループをメインにして、QTimerでrclpy.spin_once()を定期実行する。

import sys

import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist

from PySide6.QtWidgets import (
    QApplication,
    QWidget,
    QPushButton,
    QLabel,
    QVBoxLayout,
    QHBoxLayout,
)
from PySide6.QtCore import QTimer


class TeleopNode(Node):
    def __init__(self):
        super().__init__("qt_teleop_panel")

        # 変更点:
        # /cmd_vel に Twist を送る Publisher を作成
        self.publisher = self.create_publisher(Twist, "/cmd_vel", 10)

    def publish_cmd(self, linear_x: float, angular_z: float):
        msg = Twist()
        msg.linear.x = linear_x
        msg.angular.z = angular_z

        self.publisher.publish(msg)

        self.get_logger().info(
            f"publish /cmd_vel: linear.x={linear_x}, angular.z={angular_z}"
        )


class TeleopWindow(QWidget):
    def __init__(self, ros_node: TeleopNode):
        super().__init__()

        self.ros_node = ros_node

        self.setWindowTitle("Qt ROS2 Teleop Panel")
        self.resize(300, 200)

        self.status_label = QLabel("Last command: None")

        self.forward_button = QPushButton("Forward")
        self.stop_button = QPushButton("Stop")
        self.left_button = QPushButton("Left")
        self.right_button = QPushButton("Right")

        # 変更点:
        # ボタン押下時に /cmd_vel へ Twist を publishする
        self.forward_button.clicked.connect(self.on_forward_clicked)
        self.stop_button.clicked.connect(self.on_stop_clicked)
        self.left_button.clicked.connect(self.on_left_clicked)
        self.right_button.clicked.connect(self.on_right_clicked)

        main_layout = QVBoxLayout()
        main_layout.addWidget(self.status_label)

        main_layout.addWidget(self.forward_button)

        turn_layout = QHBoxLayout()
        turn_layout.addWidget(self.left_button)
        turn_layout.addWidget(self.stop_button)
        turn_layout.addWidget(self.right_button)

        main_layout.addLayout(turn_layout)

        self.setLayout(main_layout)

        # 変更点:
        # Qtのイベントループを止めないために、rclpy.spin()ではなく
        # QTimerで定期的にrclpy.spin_once()を呼ぶ
        self.spin_timer = QTimer()
        self.spin_timer.timeout.connect(self.spin_ros_once)
        self.spin_timer.start(10)

    def spin_ros_once(self):
        rclpy.spin_once(self.ros_node, timeout_sec=0.0)

    def on_forward_clicked(self):
        self.ros_node.publish_cmd(0.2, 0.0)
        self.status_label.setText("Last command: Forward")

    def on_stop_clicked(self):
        self.ros_node.publish_cmd(0.0, 0.0)
        self.status_label.setText("Last command: Stop")

    def on_left_clicked(self):
        self.ros_node.publish_cmd(0.0, 0.5)
        self.status_label.setText("Last command: Left")

    def on_right_clicked(self):
        self.ros_node.publish_cmd(0.0, -0.5)
        self.status_label.setText("Last command: Right")


def main(args=None):
    rclpy.init(args=args)

    app = QApplication(sys.argv)

    ros_node = TeleopNode()
    window = TeleopWindow(ros_node)
    window.show()

    exit_code = app.exec()

    ros_node.destroy_node()
    rclpy.shutdown()

    sys.exit(exit_code)


if __name__ == "__main__":
    main()