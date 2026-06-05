今日の課題：ROS2 Speed Alert Node

目的：
ROS2のTopic購読とParameterを短時間で復習する。

課題内容：
ROS2で速度監視ノードを作成する。

ノード名：
speed_monitor_node

購読するTopic：
/vehicle/speed

メッセージ型：
std_msgs/msg/Float32

仕様：
受信した速度がしきい値以上なら警告ログを出す。

条件：
速度 >= threshold

この条件を満たす場合：
[WARN] Speed too high: xx km/h

条件を満たさない場合：
[INFO] Speed OK: xx km/h

threshold はROS2 Parameterで指定できるようにする。

threshold のデフォルト値：
60.0

作成するパッケージ：
speed_watch_practice

作成コマンド：
ros2 pkg create speed_watch_practice --build-type ament_python --dependencies rclpy std_msgs

実行イメージ：

1. ビルド
colcon build --symlink-install
source install/setup.bash

2. ノード起動
ros2 run speed_watch_practice speed_monitor_node

3. 別ターミナルで速度送信
ros2 topic pub /vehicle/speed std_msgs/msg/Float32 "{data: 45.0}" -1

ros2 topic pub /vehicle/speed std_msgs/msg/Float32 "{data: 75.0}" -1

4. thresholdを変更して起動
ros2 run speed_watch_practice speed_monitor_node --ros-args -p threshold:=80.0

到達目標：
1. /vehicle/speed を購読できる
2. Float32の速度値を受け取れる
3. threshold parameterで警告条件を変えられる
4. ros2 topic pubで動作確認できる