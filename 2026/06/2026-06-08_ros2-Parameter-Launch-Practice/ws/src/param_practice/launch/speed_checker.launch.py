from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument


def generate_launch_description():

  #　引数受取の宣言
  speed_limit_args = DeclareLaunchArgument(
    'speed_limit',
    default_value='60',
    description='Speed limit for the speed checker node'
  )

  speed_checker_node = Node(
    package='param_practice',
    executable='speed_checker_node',
    name='speed_checker',
    parameters=[{
      # LaunchConfigurationを使用して、引数から速度制限を取得
      'speed_limit': LaunchConfiguration('speed_limit')
    }]
  )

  return LaunchDescription([
    speed_limit_args,
    speed_checker_node
  ])
  