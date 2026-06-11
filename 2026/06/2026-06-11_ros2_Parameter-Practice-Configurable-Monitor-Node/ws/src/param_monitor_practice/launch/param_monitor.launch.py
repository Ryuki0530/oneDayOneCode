from launch import LaunchDescription
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        Node(
            package="param_monitor_practice",
            executable="param_monitor_node",
            name="param_monitor",
            output="screen",
            parameters=[
                {"robot_name": "launch_robot"},
                {"alert_level": 5},
                {"message": "started from launch"},
            ],
        )
    ])
