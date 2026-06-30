# ROS2 C++ Topic Rate Monitor

## 目的

ROS2 C++で、受信したトピックの周期を監視するノードを作成する。

## 課題内容

`/sensor_value` というトピックを購読し、受信周期を計算するC++ノードを作成しなさい。

Publisher側は `std_msgs/msg/Float32` を一定周期で送信する。
Subscriber側は、受信間隔からHzを計算し、以下のように表示する。

```bash
value: 12.3, interval: 0.100 sec, rate: 10.0 Hz
```

## 要件

- ROS2 Jazzyを想定する
- 言語はC++
- パッケージ名は `topic_rate_monitor`
- Publisherノード名は `sensor_publisher`
- Subscriberノード名は `rate_monitor`
- メッセージ型は `std_msgs/msg/Float32`
- Publisherは100ms周期で値を送信する
- Subscriberは前回受信時刻との差分から周期とHzを計算する
- 初回受信時は周期計算を行わなくてよい

## 実行例

端末1:

```bash
ros2 run topic_rate_monitor sensor_publisher
```

端末2:

```bash
ros2 run topic_rate_monitor rate_monitor
```

## 確認ポイント

- `/sensor_value` がpublishされているか
- Subscriberが値を受信できているか
- 表示されるrateが約10Hzになっているか
- Publisherの周期を変更したとき、Subscriber側の表示も変わるか

## 発展課題

余裕があれば、rateが8Hz未満になったときに以下のような警告を出す。

```bash
[WARN] topic rate is too low
```

## 学習ポイント

- `rclcpp::Node`
- `create_publisher`
- `create_subscription`
- `create_wall_timer`
- ROS2における時刻取得
- トピック周期監視