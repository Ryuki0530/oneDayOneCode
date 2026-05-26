ROS2 Jazzyインストール済み想定

can関連のモジュールのインストール手法
```
sudo apt update
sudo apt install -y can-utils
sudo apt install -y ros-jazzy-ros2-socketcan
sudo apt install -y ros-jazzy-can-msgs
```

ROS2環境読み込み
```
source /opt/ros/jazzy/setup.bash
```

仮想CANインターフェース vcan0 を作る
```
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

確認
```
ip link show vcan0
```


# ROS2トピックとしてCANを確認する
ros2_socketcanの起動
```
ros2 launch ros2_socketcan socket_can_bridge.launch.xml interface:=vcan0
``` 
別ターミナルで、ソケットCANからROS2トピックに流れていることを確認
```
ros2 topic echo /from_can_bus
```

別ターミナルでCANフレームを送る
```
cansend vcan0 100#0A
```

ros2 topic echo /from_can_busが実行中のターミナルで、以下のようなCANフレームが表示されるはずです。
```
header:
  stamp:
    sec: 1779801980
    nanosec: 473811210
  frame_id: can
id: 256
is_rtr: false
is_extended: false
is_error: false
dlc: 1
data:
- 10
- 255
- 164
- 255
- 0
- 0
- 0
- 0
---
```

この手法でCANを送った再、IDとして入力した100は、0x100であるため、10進数では256と表示されます。
