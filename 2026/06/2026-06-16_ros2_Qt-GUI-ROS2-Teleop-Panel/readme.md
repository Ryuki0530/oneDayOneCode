# 今日の課題: Qt GUI ROS2 Teleop Panel

## 目的

Qtで作成したGUIをROS2ノードとして動作させ、ボタン操作からROS2トピックをpublishする。

## 使用技術

- ROS2 Jazzy
- Python
- rclpy
- PySide6 または PyQt5
- geometry_msgs/msg/Twist

## 課題内容

Qtで簡単なGUIを作成し、以下の操作ができるROS2ノードを作成せよ。

## GUI仕様

ウィンドウには以下のボタンを配置する。

- Forward
- Stop
- Left
- Right

各ボタンを押したとき、`/cmd_vel` に `geometry_msgs/msg/Twist` をpublishする。

## 動作仕様

### Forward

```text
linear.x = 0.2
angular.z = 0.0
```

### Stop

```text
linear.x = 0.0
angular.z = 0.0
```

### Left

```text
linear.x = 0.0
angular.z = 0.5
```

### Right

```text
linear.x = 0.0
angular.z = -0.5
```

## 実装条件

- ROS2パッケージ名は `qt_ros2_gui_practice`
- ノード名は `qt_teleop_panel`
- GUIのイベントループを止めずにROS2側も処理できるようにする
- `QTimer` を使って定期的に `rclpy.spin_once(node, timeout_sec=0)` を呼ぶ

## 確認方法

別ターミナルで以下を実行し、ボタン操作に応じて `/cmd_vel` が流れることを確認する。

```bash
ros2 topic echo /cmd_vel
```

## 発展課題

余裕があれば、現在最後に押したボタン名をGUI上のラベルに表示せよ。