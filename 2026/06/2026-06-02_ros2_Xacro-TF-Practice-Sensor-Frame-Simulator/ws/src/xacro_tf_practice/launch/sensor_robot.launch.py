from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import Command
from launch.substitutions import FindExecutable
from launch.substitutions import LaunchConfiguration
from launch.substitutions import PathJoinSubstitution

from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # 変更点:
    # launch引数として sensor_height と target_radius を受け取る
    sensor_height = LaunchConfiguration("sensor_height")
    target_radius = LaunchConfiguration("target_radius")

    package_share = FindPackageShare("xacro_tf_practice")

    xacro_file = PathJoinSubstitution([
        package_share,
        "xacro",
        "sensor_robot.xacro",
    ])

    # 変更点:
    # launch実行時にxacroをURDF文字列へ変換し、
    # robot_state_publisherのrobot_descriptionへ渡す
    robot_description_content = Command([
        FindExecutable(name="xacro"),
        " ",
        xacro_file,
        " ",
        "sensor_height:=",
        sensor_height,
    ])

    robot_description = {
        "robot_description": ParameterValue(
            robot_description_content,
            value_type=str
        )
    }

    return LaunchDescription([
        DeclareLaunchArgument(
            "sensor_height",
            default_value="0.8",
            description="Height of mast_link"
        ),

        DeclareLaunchArgument(
            "target_radius",
            default_value="1.0",
            description="Radius of target_frame circular motion"
        ),

        Node(
            package="robot_state_publisher",
            executable="robot_state_publisher",
            name="robot_state_publisher",
            output="screen",
            parameters=[robot_description],
        ),

        Node(
            package="xacro_tf_practice",
            executable="target_tf_broadcaster",
            name="target_tf_broadcaster",
            output="screen",
            parameters=[{
                "target_radius": ParameterValue(
                    target_radius,
                    value_type=float
                )
            }],
        ),

        Node(
            package="xacro_tf_practice",
            executable="target_tf_listener",
            name="target_tf_listener",
            output="screen",
        ),
    ])
