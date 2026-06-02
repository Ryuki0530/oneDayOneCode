cmake_minimum_required(VERSION 3.8)
project(xacro_tf_practice)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 変更点:
# C++ノードのビルドに必要なパッケージを読み込む
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)

# 変更点:
# target_tf_broadcaster ノードを作成する
add_executable(target_tf_broadcaster
  src/target_tf_broadcaster.cpp
)

ament_target_dependencies(target_tf_broadcaster
  rclcpp
  geometry_msgs
  tf2
  tf2_ros
)

# 変更点:
# target_tf_listener ノードを作成する
add_executable(target_tf_listener
  src/target_tf_listener.cpp
)

ament_target_dependencies(target_tf_listener
  rclcpp
  geometry_msgs
  tf2
  tf2_ros
)

# 変更点:
# ros2 run できるように実行ファイルをinstallする
install(TARGETS
  target_tf_broadcaster
  target_tf_listener
  DESTINATION lib/${PROJECT_NAME}
)

# 変更点:
# launchファイルとxacroファイルをinstallする
install(DIRECTORY
  launch
  xacro
  DESTINATION share/${PROJECT_NAME}
)

ament_package()
