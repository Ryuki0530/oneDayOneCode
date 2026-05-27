from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition, UnlessCondition
from launch_ros.actions import Node


def generate_launch_description():
    initial_mode_arg = DeclareLaunchArgument(
        'initial_mode', default_value='true',
        description='If true start in AUTO, otherwise MANUAL')

    initial_mode = LaunchConfiguration('initial_mode')

    node_auto = Node(
        package='mode_switch_practice',
        executable='mode_manager_node',
        name='mode_manager_node',
        output='screen',
        parameters=[{'initial_state': 'AUTO'}],
        condition=IfCondition(initial_mode),
    )

    node_manual = Node(
        package='mode_switch_practice',
        executable='mode_manager_node',
        name='mode_manager_node',
        output='screen',
        parameters=[{'initial_state': 'MANUAL'}],
        condition=UnlessCondition(initial_mode),
    )

    return LaunchDescription([
        initial_mode_arg,
        node_auto,
        node_manual,
    ])
