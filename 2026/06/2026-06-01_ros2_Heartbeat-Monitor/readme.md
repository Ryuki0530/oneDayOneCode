```markdown
# ROS2 C++ Mini Practice: Heartbeat Monitor

## 目的

ROS2 C++で、Topic購読とTimerを使った簡単な監視ノードを作成する。

## 使用環境

- Ubuntu
- ROS2 Jazzy
- C++
- rclcpp
- std_msgs

## 課題

`heartbeat_monitor_cpp` というROS2 C++パッケージを作成し、  
`heartbeat_monitor_node` というノードを実装してください。

このノードは `/heartbeat` トピックを購読し、一定時間メッセージが届かない場合に警告を出します。

## 仕様

### 購読するTopic

| Topic名 | 型 |
|---|---|
| `/heartbeat` | `std_msgs/msg/Bool` |

### 動作仕様

1. `/heartbeat` に `true` または `false` が届いたら、最後に受信した時刻を更新する。
2. 1秒ごとにTimerで監視処理を行う。
3. 最後の受信から2秒以上経過していたら、以下のようにWARNログを出す。

```text
[WARN] Heartbeat timeout
```

4. 2秒以内に受信できている場合は、以下のようにINFOログを出す。

```text
[INFO] Heartbeat OK
```

## 作成するもの

```text
heartbeat_monitor_cpp/
├── CMakeLists.txt
├── package.xml
└── src/
    └── heartbeat_monitor_node.cpp
```

## 実行例

### パッケージ作成

```bash
cd ~/ros2_ws/src
ros2 pkg create heartbeat_monitor_cpp --build-type ament_cmake --dependencies rclcpp std_msgs
```

### ビルド

```bash
cd ~/ros2_ws
colcon build --symlink-install
source install/setup.bash
```

### ノード起動

```bash
ros2 run heartbeat_monitor_cpp heartbeat_monitor_node
```

### 別ターミナルからHeartbeat送信

```bash
ros2 topic pub /heartbeat std_msgs/msg/Bool "{data: true}" -r 1
```

## 確認ポイント

- `/heartbeat` を送信している間は `Heartbeat OK` が表示される
- `ros2 topic pub` を止めると、約2秒後から `Heartbeat timeout` が表示される
- `rclcpp::Subscription` を使えている
- `rclcpp::TimerBase` を使えている
- `this->now()` で現在時刻を取得できている

## ヒント

最後に受信した時刻をメンバ変数として保持するとよい。

例：

```cpp
rclcpp::Time last_heartbeat_time_;
```

現在時刻との差分は次のように計算できる。

```cpp
auto elapsed = this->now() - last_heartbeat_time_;
```

秒数は次のように取得できる。

```cpp
elapsed.seconds()
```

## 余裕があれば

- `/heartbeat` の値が `false` のときは `Heartbeat received, but status is false` とWARNログを出す
- タイムアウト秒数をパラメータ `timeout_sec` で変更できるようにする
```