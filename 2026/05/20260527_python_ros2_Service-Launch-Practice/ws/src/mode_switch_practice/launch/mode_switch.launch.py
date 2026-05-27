from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch_ros.parameter_descriptions import ParameterValue


def generate_launch_description():
    initial_mode_arg = DeclareLaunchArgument(
        'initial_mode',
        default_value='false',
        description='If true start in AUTO, otherwise MANUAL'
    )

    initial_mode = LaunchConfiguration('initial_mode')

    mode_manager_node = Node(
        package='mode_switch_practice',
        executable='mode_manager_node',
        name='mode_manager_node',
        output='screen',
        parameters=[
            {
                # 変更点: mode_manager_node.py 側のパラメータ名と合わせる
                # 変更点: bool型として渡す
                'initial_mode': ParameterValue(initial_mode, value_type=bool),
            }
        ],
    )

    return LaunchDescription([
        initial_mode_arg,
        mode_manager_node,
    ])