# 今日の課題: Xacro TF Practice - Sensor Frame Simulator

## 目的

ROS 2 Jazzy 環境で、以下を一通り練習する。

- URDFのリンク・ジョイント構造
- xacroによるURDFの部品化
- xacroの引数をlaunchファイルから渡す方法
- robot_state_publisherによる静的なロボットモデルのTF配信
- tf2 broadcasterによる動的フレーム配信
- tf2 listenerによるフレーム間座標の取得

## 想定環境

- Windows 11 + WSL2
- Ubuntu 24.04
- ROS 2 Jazzy
- ワークスペース: `~/ros2_lecture_ws`

## 課題タイトル

Xacro TF Practice - Sensor Frame Simulator

## 作成するROS 2パッケージ

パッケージ名は以下とする。

    xacro_tf_practice

ビルドタイプは `ament_cmake` とする。

## 作成する構成

以下のような構成を作成する。

    ros2_lecture_ws/
    └── src/
        └── xacro_tf_practice/
            ├── CMakeLists.txt
            ├── package.xml
            ├── xacro/
            │   ├── common_parts.xacro
            │   └── sensor_robot.xacro
            ├── launch/
            │   └── sensor_robot.launch.py
            └── src/
                ├── target_tf_broadcaster.cpp
                └── target_tf_listener.cpp

## 実装内容

### 1. xacroでロボットモデルを作成する

`sensor_robot.xacro` に、以下のリンクを持つロボットを作成する。

- `base_link`
- `mast_link`
- `sensor_link`

構造は以下とする。

    base_link
    └── mast_link
        └── sensor_link

## 2. base_link

`base_link` はロボットの土台とする。

形状は直方体とし、サイズは以下とする。

    x: 0.4
    y: 0.3
    z: 0.1

色は任意でよい。

## 3. mast_link

`mast_link` はセンサーを支える支柱とする。

形状は直方体または円柱とする。

高さはxacro引数 `sensor_height` で変更できるようにする。

デフォルト値は以下とする。

    sensor_height = 0.8

## 4. sensor_link

`sensor_link` はセンサー本体とする。

形状は小さな直方体とし、サイズは以下とする。

    x: 0.2
    y: 0.1
    z: 0.1

`sensor_link` は `mast_link` の上に固定する。

## 5. common_parts.xacro

`common_parts.xacro` には、リンク作成用のxacroマクロを1つ以上定義する。

例:

    <xacro:macro name="box_link" params="name x y z color">
      ...
    </xacro:macro>

`sensor_robot.xacro` では、この `common_parts.xacro` をincludeして使うこと。

## 6. launchファイルを作成する

`sensor_robot.launch.py` を作成し、以下を起動する。

- `robot_state_publisher`
- `target_tf_broadcaster`
- `target_tf_listener`
- 可能であれば `rviz2`

launchファイルでは、以下の引数を受け取れるようにする。

    sensor_height
    target_radius

デフォルト値は以下とする。

    sensor_height = 0.8
    target_radius = 1.0

`sensor_height` はxacroに渡す。

`target_radius` は `target_tf_broadcaster` ノードのROSパラメータとして渡す。

## 7. target_tf_broadcaster.cpp

`target_tf_broadcaster.cpp` では、以下のTFを配信する。

    parent frame: base_link
    child frame : target_frame

`target_frame` は `base_link` の周囲を円運動するものとする。

仕様:

- 半径はROSパラメータ `target_radius` から取得する
- デフォルト値は `1.0`
- 0.1秒ごとにTFを配信する
- z座標は `0.0` とする
- x, y座標は時間変化する
- 回転は単位クォータニオンでよい

例:

    x = radius * cos(t)
    y = radius * sin(t)
    z = 0.0

## 8. target_tf_listener.cpp

`target_tf_listener.cpp` では、以下のTFを取得する。

    target frame: base_link
    source frame: target_frame

取得できた場合、0.5秒ごとに以下のようなログを出す。

    base_link -> target_frame: x=0.54 y=0.84 distance=1.00

距離は以下で計算する。

    distance = sqrt(x*x + y*y)

TFがまだ取得できない場合は、例外をcatchしてWARNログを出す。

## 9. ビルド手順

必要に応じて、以下をインストールする。

    sudo apt update
    sudo apt install ros-jazzy-xacro ros-jazzy-tf2-ros ros-jazzy-robot-state-publisher
    sudo apt install liburdfdom-tools

ワークスペースでビルドする。

    cd ~/ros2_lecture_ws
    rosdep install -i --from-path src --rosdistro jazzy -y
    colcon build --packages-select xacro_tf_practice
    source install/setup.bash

## 10. 動作確認

### xacroをURDFへ変換できること

    ros2 run xacro xacro \
      ~/ros2_lecture_ws/src/xacro_tf_practice/xacro/sensor_robot.xacro \
      sensor_height:=1.2 \
      > /tmp/sensor_robot.urdf

## URDFチェック

    check_urdf /tmp/sensor_robot.urdf

以下のように、root linkが表示されればよい。

    robot name is: sensor_robot
    Successfully Parsed XML
    root Link: base_link

## launch実行

    ros2 launch xacro_tf_practice sensor_robot.launch.py

高さやターゲット半径を変更して実行する。

    ros2 launch xacro_tf_practice sensor_robot.launch.py sensor_height:=1.2 target_radius:=1.5

## tf2_echoで確認

別ターミナルで以下を実行する。

    source /opt/ros/jazzy/setup.bash
    source ~/ros2_lecture_ws/install/setup.bash
    ros2 run tf2_ros tf2_echo base_link target_frame

`target_frame` のx, y座標が時間変化していれば成功。

## 11. 完成条件

以下を満たせば完成とする。

- `sensor_robot.xacro` をURDFへ変換できる
- `check_urdf` が成功する
- launchファイルから `sensor_height` を変更できる
- `target_tf_broadcaster` が `base_link -> target_frame` を配信する
- `target_tf_listener` が `target_frame` の座標と距離をログ出力する
- `target_radius:=1.5` のように変更すると、出力される距離もおよそ `1.5` になる

## 12. 余裕があれば

以下の拡張に挑戦する。

### 拡張1

`sensor_link` から `camera_optical_frame` へのstatic TFを追加する。

構造:

    sensor_link
    └── camera_optical_frame

### 拡張2

`target_radius` が `1.2` 以上の場合、listener側でWARNログを出す。

例:

    [WARN] target is far: distance=1.50

### 拡張3

RVizで以下のフレームが見えることを確認する。

- `base_link`
- `mast_link`
- `sensor_link`
- `target_frame`
- `camera_optical_frame`