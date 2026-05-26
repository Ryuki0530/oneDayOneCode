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