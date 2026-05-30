# ROS2 Interface Message Practice

## 今日の目的

C++でROS2のカスタムメッセージを定義し、それを別パッケージのPublisher / Subscriberノードから利用する。

今回は、ロボットや車両の状態を表す独自メッセージを作成する。

---

## 使用環境

- Ubuntu
- ROS2 Jazzy
- C++
- ament_cmake

---

## 作成するワークスペース

```bash
mkdir -p ~/Desktop/oneDayOneCode/2026/05/20260529_cpp_ros2_interface_message/ws/src
cd ~/Desktop/oneDayOneCode/2026/05/20260529_cpp_ros2_interface_message/ws/src
```

---

## 課題1: インターフェイス定義用パッケージを作成する

パッケージ名は以下にする。

```txt
robot_interfaces
```

作成コマンド例:

```bash
ros2 pkg create --build-type ament_cmake robot_interfaces
```

---

## 課題2: カスタムメッセージを定義する

`robot_interfaces/msg/RobotStatus.msg` を作成する。

メッセージ内容は以下とする。

```txt
# RobotStatus.msg
# ロボットの状態を表すカスタムメッセージ

string robot_name
int32 battery_percent
float32 temperature
bool is_emergency

int32 NORMAL=0
int32 WARNING=1
int32 ERROR=2
int32 status_level
```

---

## 課題3: robot_interfaces の package.xml を編集する

`package.xml` に以下の依存関係を追加する。

```xml
<build_depend>rosidl_default_generators</build_depend>
<exec_depend>rosidl_default_runtime</exec_depend>
<member_of_group>rosidl_interface_packages</member_of_group>
```

---

## 課題4: robot_interfaces の CMakeLists.txt を編集する

`CMakeLists.txt` に以下を追加する。

```cmake
find_package(rosidl_default_generators REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/RobotStatus.msg"
)
```

---

## 課題5: メッセージをビルドして確認する

ワークスペース直下で以下を実行する。

```bash
cd ~/Desktop/oneDayOneCode/2026/05/20260529_cpp_ros2_interface_message/ws
colcon build --packages-select robot_interfaces
source install/setup.bash
ros2 interface show robot_interfaces/msg/RobotStatus
```

以下のような内容が表示されれば成功。

```txt
string robot_name
int32 battery_percent
float32 temperature
bool is_emergency
int32 NORMAL=0
int32 WARNING=1
int32 ERROR=2
int32 status_level
```

---

## 課題6: カスタムメッセージを使うC++パッケージを作成する

パッケージ名は以下にする。

```txt
robot_status_cpp
```

作成コマンド例:

```bash
cd ~/Desktop/oneDayOneCode/2026/05/20260529_cpp_ros2_interface_message/ws/src
ros2 pkg create --build-type ament_cmake robot_status_cpp --dependencies rclcpp robot_interfaces
```

---

## 課題7: Publisherノードを作成する

`robot_status_cpp/src/status_publisher.cpp` を作成する。

仕様:

- ノード名: `status_publisher`
- トピック名: `/robot/status`
- メッセージ型: `robot_interfaces::msg::RobotStatus`
- 1秒ごとに状態をPublishする
- `battery_percent` は100から1ずつ減らす
- `battery_percent` が50以上なら `NORMAL`
- `battery_percent` が20以上50未満なら `WARNING`
- `battery_percent` が20未満なら `ERROR`
- `battery_percent` が10未満なら `is_emergency` を true にする

---

## 課題8: Subscriberノードを作成する

`robot_status_cpp/src/status_subscriber.cpp` を作成する。

仕様:

- ノード名: `status_subscriber`
- トピック名: `/robot/status`
- メッセージ型: `robot_interfaces::msg::RobotStatus`
- 受信した内容をログ表示する
- `is_emergency == true` の場合は `RCLCPP_ERROR` で表示する
- `status_level == WARNING` の場合は `RCLCPP_WARN` で表示する
- それ以外は `RCLCPP_INFO` で表示する

---

## 課題9: robot_status_cpp の CMakeLists.txt を編集する

以下のように、2つの実行ファイルを登録する。

```cmake
find_package(rclcpp REQUIRED)
find_package(robot_interfaces REQUIRED)

add_executable(status_publisher src/status_publisher.cpp)
ament_target_dependencies(status_publisher rclcpp robot_interfaces)

add_executable(status_subscriber src/status_subscriber.cpp)
ament_target_dependencies(status_subscriber rclcpp robot_interfaces)

install(TARGETS
  status_publisher
  status_subscriber
  DESTINATION lib/${PROJECT_NAME}
)
```

---

## 課題10: ビルドして実行する

```bash
cd ~/Desktop/oneDayOneCode/2026/05/20260529_cpp_ros2_interface_message/ws
colcon build
source install/setup.bash
```

ターミナル1:

```bash
ros2 run robot_status_cpp status_publisher
```

ターミナル2:

```bash
source ~/Desktop/oneDayOneCode/2026/05/20260529_cpp_ros2_interface_message/ws/install/setup.bash
ros2 run robot_status_cpp status_subscriber
```

---

## 確認コマンド

トピック一覧を確認する。

```bash
ros2 topic list
```

トピックの型を確認する。

```bash
ros2 topic info /robot/status
```

メッセージ内容を直接確認する。

```bash
ros2 topic echo /robot/status
```

インターフェイス定義を確認する。

```bash
ros2 interface show robot_interfaces/msg/RobotStatus
```

---

## 学習ポイント

- ROS2のメッセージは `.msg` ファイルで定義する
- メッセージ型名はアッパーキャメルケースにする
- メッセージ内の変数名はスネークケースにする
- C++では `robot_interfaces::msg::RobotStatus` のように使う
- カスタムメッセージを使うには、定義用パッケージと利用側パッケージを分けると管理しやすい
- `ros2 interface show` でメッセージ構造を確認できる
- `rosidl_generate_interfaces()` によって `.msg` からC++用の型が生成される

---

## 余裕があれば追加課題

### 追加課題1

`RobotStatus.msg` に以下を追加する。

```txt
float32[3] position
string<=20 mode_name
```

Publisher側で適当な値を入れ、Subscriber側で表示する。

---

### 追加課題2

`robot_name` を `so101_arm` にして、ロボットアームの状態監視ノードっぽくする。

---

### 追加課題3

`temperature` が70.0以上になったら `WARNING`、90.0以上になったら `ERROR` にする。