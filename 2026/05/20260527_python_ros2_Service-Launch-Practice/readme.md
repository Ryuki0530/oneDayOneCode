# ROS2 Python Service Launch Practice

## 目的

ROS2のPythonノードで、以下を練習する。

- Service Server
- Service Client
- `std_srvs/srv/SetBool`
- Launchファイル
- Launch引数
- ROS2パラメータ

---

## 課題名

**ROS2 Mode Switch Service**

---

## 開発環境

- Ubuntu
- ROS2 Jazzy
- Python
- VSCode

---

## 作成するパッケージ

```bash
ros2 pkg create mode_switch_practice --build-type ament_python --dependencies rclpy std_srvs
```

---

## 作成するノード

### 1. `mode_manager_node.py`

ロボットの動作モードを管理するService Serverを作成する。

このノードは、現在のモードを内部状態として持つ。

- `True` のとき: `AUTO`
- `False` のとき: `MANUAL`

使用するサービス型は以下。

```python
std_srvs.srv.SetBool
```

サービス名は以下。

```text
/set_auto_mode
```

リクエストの `data` が `True` の場合、モードを `AUTO` に変更する。

リクエストの `data` が `False` の場合、モードを `MANUAL` に変更する。

レスポンスは以下のようにする。

- `success`: 常に `True`
- `message`: `Mode changed to AUTO` または `Mode changed to MANUAL`

また、2秒ごとに現在のモードをログ出力する。

例:

```text
Current mode: MANUAL
Current mode: AUTO
```

---

### 2. `mode_client_node.py`

Service Clientを作成する。

このノードはコマンドライン引数からモードを受け取り、Service Serverへリクエストを送信する。

実行例:

```bash
ros2 run mode_switch_practice mode_client_node auto
```

この場合、`/set_auto_mode` に `True` を送信する。

```bash
ros2 run mode_switch_practice mode_client_node manual
```

この場合、`/set_auto_mode` に `False` を送信する。

引数が `auto` または `manual` 以外の場合は、エラーメッセージを出して終了する。

---

## Launchファイル

以下のLaunchファイルを作成する。

```text
launch/mode_switch.launch.py
```

このLaunchファイルでは、`mode_manager_node` を起動する。

Launch引数として `initial_mode` を受け取れるようにする。

```bash
ros2 launch mode_switch_practice mode_switch.launch.py initial_mode:=true
```

`initial_mode:=true` の場合、初期状態を `AUTO` にする。

```bash
ros2 launch mode_switch_practice mode_switch.launch.py initial_mode:=false
```

`initial_mode:=false` の場合、初期状態を `MANUAL` にする。

---

## パラメータ

`mode_manager_node.py` は以下のROS2パラメータを持つ。

```text
initial_mode
```

型はbool。

デフォルト値は `False`。

---

## 実行手順

### ビルド

```bash
colcon build --symlink-install
source install/setup.bash
```

### サーバー起動

```bash
ros2 launch mode_switch_practice mode_switch.launch.py initial_mode:=false
```

### 別ターミナルでクライアント実行

```bash
source install/setup.bash
ros2 run mode_switch_practice mode_client_node auto
```

```bash
ros2 run mode_switch_practice mode_client_node manual
```

---

## 期待される動作

### サーバー側

```text
Current mode: MANUAL
Current mode: MANUAL
Mode changed to AUTO
Current mode: AUTO
Current mode: AUTO
Mode changed to MANUAL
Current mode: MANUAL
```

### クライアント側

```text
Request sent: AUTO
Response: Mode changed to AUTO
```

または

```text
Request sent: MANUAL
Response: Mode changed to MANUAL
```

---

## 余裕があれば追加実装

### 発展1

Service名をLaunch引数で変更できるようにする。

例:

```bash
ros2 launch mode_switch_practice mode_switch.launch.py service_name:=/robot/set_auto_mode
```

---

### 発展2

現在のモードをTopicとして配信する。

Topic名:

```text
/current_mode
```

型:

```python
std_msgs.msg.String
```

配信内容:

```text
AUTO
```

または

```text
MANUAL
```

---

### 発展3

`mode_client_node.py` 側でも、接続待機中に以下のようなログを出す。

```text
Waiting for service /set_auto_mode ...
```

---

## 学習ポイント

- Serviceは「要求を送って、応答を受け取る」通信に向いている
- Topicは「継続的に状態を配信する」通信に向いている
- Launchを使うと、ノード起動時のパラメータや構成をまとめて管理できる
- Pythonノードでは `setup.py` に実行コマンドを登録する必要がある