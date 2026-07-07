# OneDayOneCode
## ROS2 Control Minimal URDF Practice

### 目的

Gazeboを使わずに、ROS2 Controlで必要になる以下の要素を自作して理解する。

- URDF
- ros2_controlタグ
- controller設定YAML
- controller_manager
- joint_state_broadcaster

---

## 課題

1自由度の簡単なロボットアームをURDFで作成する。

構成は以下とする。

base_link
  |
  | revolute joint
  |
arm_link

---

## 要件

### 1. URDFを作成する

以下を含めること。

- base_link
- arm_link
- revolute joint
- joint limit
- ros2_controlタグ

joint名は以下とする。

joint1

---

### 2. controller設定YAMLを作成する

以下のControllerを定義する。

- joint_state_broadcaster
- joint_trajectory_controller

対象jointは joint1 とする。

---

### 3. launchファイルを作成する

launchファイルでは以下を起動する。

- robot_state_publisher
- ros2_control_node
- joint_state_broadcaster spawner
- joint_trajectory_controller spawner

---

### 4. 起動後に確認する

以下のコマンドで確認する。

ros2 control list_controllers

ros2 control list_hardware_interfaces

ros2 topic echo /joint_states

---

### 5. まとめる

以下を100〜200字程度で説明する。

- URDFに書くros2_controlタグの役割
- controller_managerの役割
- joint_state_broadcasterの役割
- joint_trajectory_controllerの役割