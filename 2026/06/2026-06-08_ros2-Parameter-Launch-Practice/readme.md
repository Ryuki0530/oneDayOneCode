# OneDayOneCode
# ROS2 Parameter + Launch Practice

## 問題

ROS2 C++で、Launchファイルからパラメータを渡して動作を変えるノードを作成せよ。

## パッケージ名

param_practice

## ノード名

speed_checker_node

## パラメータ

speed_limit : int型

デフォルト値:
60

## 動作

ノード起動時に speed_limit を読み取り、以下のようにログ出力する。

speed_limit が 80 以上:
[WARN] speed_limit=80 high speed threshold

speed_limit が 80 未満:
[INFO] speed_limit=60 normal speed threshold

## 実行方法1: ros2 run

ros2 run param_practice speed_checker_node

ros2 run param_practice speed_checker_node --ros-args -p speed_limit:=80

## 実行方法2: launch

ros2 launch param_practice speed_checker.launch.py

ros2 launch param_practice speed_checker.launch.py speed_limit:=80

## 条件

- ROS2 Jazzy
- C++
- rclcpp
- declare_parameter() を使うこと
- get_parameter() を使うこと
- LaunchConfiguration を使うこと
- DeclareLaunchArgument を使うこと

## 余力があれば

speed_limit だけでなく、current_speed もlaunch引数で渡し、
current_speed >= speed_limit のときだけWARNを出すようにせよ。