# ROS2 Parameter Practice - Configurable Monitor Node

## 今日の課題

ROS 2のParameter機能を使って、起動時・実行中に設定値を変更できるノードを作成する。

## 目的

以下を予習する。

- C++ノード内でparameterを宣言する
- parameterに説明文・read_only・dynamic_typingを設定する
- launchファイルからparameterの初期値を上書きする
- ros2 param list / get / set / describe / delete を試す

## 作成するパッケージ

パッケージ名:

```bash
param_monitor_practice
```

ノード名:

```bash
param_monitor
```

実行ファイル名:

```bash
param_monitor_node
```

## 仕様

C++で `param_monitor` ノードを作成する。

このノードは1秒ごとに以下のparameterを表示する。

| parameter名 | 型 | 初期値 | 仕様 |
|---|---|---:|---|
| robot_name | string | "robot_01" | 実行中に変更できる |
| alert_level | int | 3 | 読み取り専用 |
| message | string | "hello" | 実行中に変更・削除できる |

## 実装条件

1. `declare_parameter()` を使って3つのparameterを宣言すること。

2. `rcl_interfaces::msg::ParameterDescriptor` を使って、各parameterにdescriptionを設定すること。

3. `alert_level` は `read_only = true` にすること。

4. `message` は `dynamic_typing = true` にすること。

5. タイマコールバックで、1秒ごとに以下を表示すること。

```text
robot_name: xxx
alert_level: xxx
message: xxx
```

6. `message` が削除されている場合は、WARNログで以下を出すこと。

```text
message parameter is not declared
```

## launchファイル

以下のlaunchファイルを作成する。

```text
launch/param_monitor.launch.py
```

launchから以下の値を与える。

| parameter名 | launchで与える値 |
|---|---|
| robot_name | "launch_robot" |
| alert_level | 5 |
| message | "started from launch" |

## 確認コマンド

### 1. ビルド

```bash
colcon build --packages-select param_monitor_practice
source install/setup.bash
```

### 2. launchで起動

```bash
ros2 launch param_monitor_practice param_monitor.launch.py
```

### 3. 別ターミナルでparameter一覧を確認

```bash
ros2 param list
```

期待する確認内容:

```text
/param_monitor:
  robot_name
  alert_level
  message
```

### 4. parameterの値を取得

```bash
ros2 param get /param_monitor robot_name
ros2 param get /param_monitor alert_level
ros2 param get /param_monitor message
```

### 5. 実行中に値を変更

```bash
ros2 param set /param_monitor robot_name robot_02
ros2 param set /param_monitor message running
```

ノード側のログが変化すればOK。

### 6. 読み取り専用parameterを変更して失敗を確認

```bash
ros2 param set /param_monitor alert_level 10
```

期待する結果:

```text
read-onlyなので変更に失敗する
```

### 7. parameterの説明を確認

```bash
ros2 param describe /param_monitor robot_name
ros2 param describe /param_monitor alert_level
ros2 param describe /param_monitor message
```

### 8. messageを削除

```bash
ros2 param delete /param_monitor message
```

ノード側で以下のようなWARNが出ればOK。

```text
message parameter is not declared
```

## 完了条件

以下を確認できれば完了。

- launchで指定した値がC++側の初期値を上書きしている
- ros2 param getで値を取得できる
- ros2 param setで実行中に値を変更できる
- read_onlyのparameterは変更できない
- dynamic_typing=trueのparameterはdeleteできる
- delete後にC++ノード側でWARNを出せる