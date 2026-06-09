# ROS2 Lifecycle Sensor Node Practice

## 目的

ROS2のLifecycle Nodeを使い、ノードの状態に応じて動作が変わる仕組みを理解する。

通常のノードは起動するとすぐ処理を開始するが、Lifecycle Nodeでは以下のように状態を管理できる。

- unconfigured
- inactive
- active
- finalized

今回は、active状態のときだけメッセージをpublishするセンサーノードを作成する。

---

## 使用環境

- Ubuntu
- ROS2 Jazzy
- C++
- colcon
- VSCode

---

## 作成するパッケージ

パッケージ名:

```bash
lifecycle_practice
```

ノード名:

```bash
managed_sensor_node
```

---

## パッケージ作成コマンド

```bash
cd ~/ros2_ws/src

ros2 pkg create lifecycle_practice \
  --build-type ament_cmake \
  --dependencies rclcpp rclcpp_lifecycle lifecycle_msgs std_msgs
```

---

## 実装する仕様

`ManagedSensorNode` クラスを作成する。

このクラスは `rclcpp_lifecycle::LifecycleNode` を継承する。

---

## ノードの動作

### 1. configureされたとき

`on_configure()` が呼ばれる。

この中で以下を行う。

- publisherを作成する
- timerを作成する
- ログに `"configured"` と表示する

ただし、この時点ではまだpublishしない。

---

### 2. activateされたとき

`on_activate()` が呼ばれる。

この中で以下を行う。

- publisherをactivateする
- ログに `"activated"` と表示する

active状態の間だけ、1秒ごとに以下の文字列をpublishする。

```txt
sensor is running
```

publish先topic:

```bash
/sensor_status
```

型:

```bash
std_msgs/msg/String
```

---

### 3. deactivateされたとき

`on_deactivate()` が呼ばれる。

この中で以下を行う。

- publisherをdeactivateする
- ログに `"deactivated"` と表示する

inactive状態ではpublishされないことを確認する。

---

### 4. cleanupされたとき

`on_cleanup()` が呼ばれる。

この中で以下を行う。

- publisherを破棄する
- timerを破棄する
- ログに `"cleaned up"` と表示する

---

### 5. shutdownされたとき

`on_shutdown()` が呼ばれる。

ログに `"shutdown"` と表示する。

---

## 実行確認

### ビルド

```bash
cd ~/ros2_ws
colcon build --symlink-install
source install/setup.bash
```

---

### ノード起動

```bash
ros2 run lifecycle_practice managed_sensor_node
```

---

### 別ターミナルで状態確認

```bash
ros2 lifecycle get /managed_sensor_node
```

最初は以下のような状態になる。

```txt
unconfigured
```

---

### configureへ遷移

```bash
ros2 lifecycle set /managed_sensor_node configure
```

状態確認:

```bash
ros2 lifecycle get /managed_sensor_node
```

期待結果:

```txt
inactive
```

---

### activeへ遷移

```bash
ros2 lifecycle set /managed_sensor_node activate
```

状態確認:

```bash
ros2 lifecycle get /managed_sensor_node
```

期待結果:

```txt
active
```

---

### topic確認

```bash
ros2 topic echo /sensor_status
```

期待される出力例:

```txt
data: sensor is running
---
data: sensor is running
---
```

---

### inactiveへ戻す

```bash
ros2 lifecycle set /managed_sensor_node deactivate
```

この状態では `/sensor_status` に新しいメッセージが流れないことを確認する。

---

## 追加課題

余裕があれば、publishする文字列をparameterで変更できるようにする。

parameter名:

```bash
status_message
```

デフォルト値:

```txt
sensor is running
```

例:

```bash
ros2 run lifecycle_practice managed_sensor_node --ros-args -p status_message:="camera is active"
```

---

## 完了条件

以下を確認できれば完了。

- Lifecycle Nodeを作成できる
- `configure` でinactiveになる
- `activate` でpublishが始まる
- `deactivate` でpublishが止まる
- `/sensor_status` を `ros2 topic echo` で確認できる