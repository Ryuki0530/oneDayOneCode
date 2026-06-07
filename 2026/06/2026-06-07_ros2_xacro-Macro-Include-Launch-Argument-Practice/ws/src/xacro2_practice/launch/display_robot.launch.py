from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import Command, LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # 変更点:
    # launch引数 sensor_height を定義する
    sensor_height = LaunchConfiguration('sensor_height')

    sensor_height_arg = DeclareLaunchArgument(
        'sensor_height',
        default_value='0.3',
        description='Height of the three sensor links'
    )

    # 変更点:
    # install後のパッケージ共有ディレクトリから robot.xacro を探す
    xacro_file = PathJoinSubstitution([
        FindPackageShare('xacro2_practice'),
        'xacro',
        'robot.xacro'
    ])

    # 変更点:
    # xacro コマンドを launch 内で実行し、
    # robot_description パラメータとして robot_state_publisher に渡す
    robot_description = Command([
        'xacro ',
        xacro_file,
        ' sensor_height:=',
        sensor_height
    ])

    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        name='robot_state_publisher',
        output='screen',
        parameters=[
            {
                'robot_description': robot_description
            }
        ]
    )

    rviz2_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen'
    )

    return LaunchDescription([
        sensor_height_arg,
        robot_state_publisher_node,
        rviz2_node
    ])
