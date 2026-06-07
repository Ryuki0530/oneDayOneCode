# 今日の課題: ROS2 xacro Macro / Include / Launch Argument Practice

## 目的

ROS2のxacroで以下を練習する。

- xacro:macro を使って同じ形状のlink/joint定義を再利用する
- xacro:include を使って別ファイルのマクロを読み込む
- launchファイルからxacroへ引数を渡し、ロボットモデルの形を変更する
- robot_state_publisher と RViz2 で生成されたURDFを確認する

## 想定環境

- Ubuntu / WSL2 Ubuntu
- ROS2 Jazzy
- VSCode
- ワークスペース名: xacro2_practice_ws
- パッケージ名: xacro2_practice

## 課題内容

### 1. パッケージを作成する

以下の構成になるようにROS2パッケージを作成する。

xacro2_practice_ws/
  src/
    xacro2_practice/
      xacro/
        parts.xacro
        robot.xacro
      launch/
        display_robot.launch.py
      CMakeLists.txt
      package.xml

パッケージは ament_cmake で作成すること。

必要に応じて以下を入れる。

sudo apt install ros-jazzy-xacro ros-jazzy-robot-state-publisher ros-jazzy-rviz2

### 2. parts.xacro を作成する

xacro/parts.xacro に、箱型センサーを作るマクロを定義する。

要件:

- マクロ名: sensor_box
- 引数:
  - prefix
  - parent
  - x
  - y
  - z
- 作成するもの:
  - fixed joint
  - box形状のlink
- link名は `${prefix}_link`
- joint名は `${prefix}_joint`
- boxサイズは `0.15 0.08 0.05`
- 親linkは引数 parent で指定する
- 位置は x, y, z 引数で指定する

### 3. robot.xacro を作成する

xacro/robot.xacro を作成する。

要件:

- parts.xacro を include する
- base_link を作成する
- base_linkには箱型のvisualを持たせる
  - サイズ: `0.4 0.3 0.1`
- xacro引数 `sensor_height` を定義する
  - デフォルト値: `0.2`
- sensor_box マクロを3回使い、以下の3つのセンサーを作る

| prefix | x | y | z |
|---|---:|---:|---:|
| front_sensor | 0.25 | 0.0 | sensor_height |
| left_sensor | 0.0 | 0.2 | sensor_height |
| right_sensor | 0.0 | -0.2 | sensor_height |

z座標には xacro引数 `sensor_height` を使うこと。

### 4. xacroをURDFに変換して確認する

以下のように、sensor_height の値を変えてURDFを生成する。

例:

ros2 run xacro xacro src/xacro2_practice/xacro/robot.xacro sensor_height:=0.2 > /tmp/robot_low.urdf

ros2 run xacro xacro src/xacro2_practice/xacro/robot.xacro sensor_height:=0.8 > /tmp/robot_high.urdf

確認すること:

- `/tmp/robot_low.urdf` と `/tmp/robot_high.urdf` が生成される
- sensor_height の値によって、3つのセンサーlinkのz座標が変化している

### 5. launchファイルを作成する

launch/display_robot.launch.py を作成する。

要件:

- launch引数 `sensor_height` を受け取る
- デフォルト値は `0.3`
- xacro/robot.xacro に `sensor_height:=...` を渡す
- robot_state_publisher を起動する
- RViz2 を起動する

### 6. CMakeLists.txt を編集する

以下のディレクトリがinstallされるようにする。

- xacro
- launch

### 7. ビルドして実行する

ワークスペース直下で実行する。

colcon build --symlink-install
source install/setup.bash

デフォルト値で起動:

ros2 launch xacro2_practice display_robot.launch.py

高さを変更して起動:

ros2 launch xacro2_practice display_robot.launch.py sensor_height:=1.0

## 完了条件

以下を満たせば完了。

- xacro:macro を使ってセンサーlink/jointを再利用できている
- xacro:include で parts.xacro を robot.xacro から読み込めている
- sensor_height の値を変えると、3つのセンサーの高さが変わる
- ros2 launch で robot_state_publisher と RViz2 が起動する
- RViz2上で base_link と3つのsensor linkが確認できる

## 余裕があれば

以下の拡張を行う。

1. sensor_box に color 引数を追加する
2. front / left / right のセンサー色を変える
3. sensor_height だけでなく sensor_offset_x もlaunch引数から変更できるようにする
4. 生成されたURDF内で、各link名とjoint名がどう展開されているか確認する