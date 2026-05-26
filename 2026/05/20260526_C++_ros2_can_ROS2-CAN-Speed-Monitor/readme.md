# OneDayOneCode: ROS2 CAN Speed Monitor

## 概要

Ubuntu + ROS2 Jazzy 環境で、仮想CANインターフェース `vcan0` から流れてくるCANメッセージをROS2ノードで受信し、車速を監視するプログラムを作成してください。

今回は、`ros2_socketcan` を用いてCANフレームをROS2トピックとして受信し、C++で作成したノード `speed_monitor_node` で車速情報を読み取ります。

---

## 使用環境

- Ubuntu
- ROS2 Jazzy
- C++
- SocketCAN
- vcan0
- ros2_socketcan

---

## 作成するROS2パッケージ

パッケージ名は以下とします。

```text
can_ros2_practice
```

作成するノード名は以下とします。

```text
speed_monitor_node
```

---

## 入力

`ros2_socketcan` により、CANバスから受信したCANフレームが以下のトピックに流れているものとします。

```text
/from_can_bus
```

`speed_monitor_node` はこのトピックを購読してください。

---

## CANメッセージ仕様

今回扱うCAN IDは以下です。

| CAN ID | 内容 |
|---|---|
| 0x100 | 車速 |

CAN ID `0x100` のCANフレームでは、`data[0]` に車速が格納されているものとします。

例：

```text
CAN ID: 0x100
data[0]: 45
```

この場合、車速は `45 km/h` と解釈します。

---

## 実装する処理

`speed_monitor_node` は `/from_can_bus` を購読し、受信したCANフレームのIDを確認してください。

CAN IDが `0x100` の場合、`data[0]` を車速として取り出します。

車速が `60 km/h` 以上の場合は、WARNログを出力してください。

```text
[WARN] Speed too high: 72 km/h
```

車速が `60 km/h` 未満の場合は、INFOログを出力してください。

```text
[INFO] Speed: 45 km/h
```

CAN IDが `0x100` 以外の場合は、無視して構いません。

---

## 実行例

以下のようなCANフレームを送信した場合、

```bash
cansend vcan0 100#2D
```

`data[0] = 0x2D = 45` なので、以下のようなログを出力します。

```text
[INFO] Speed: 45 km/h
```

以下のようなCANフレームを送信した場合、

```bash
cansend vcan0 100#48
```

`data[0] = 0x48 = 72` なので、以下のようなログを出力します。

```text
[WARN] Speed too high: 72 km/h
```

---

## 発展課題

余裕があれば、以下のCAN IDにも対応してください。

| CAN ID | 内容 | 解釈 |
|---|---|---|
| 0x101 | ブレーキ状態 | data[0] が 0 ならOFF、1ならON |
| 0x102 | ステアリング角 | data[0] を符号付き整数として角度とみなす |

さらに、デコードした値を以下のROS2トピックにpublishしてください。

| トピック名 | 型 | 内容 |
|---|---|---|
| /vehicle/speed | std_msgs/msg/Int32 | 車速 |
| /vehicle/brake | std_msgs/msg/Bool | ブレーキ状態 |
| /vehicle/steering | std_msgs/msg/Int32 | ステアリング角 |

---

## 目標

この課題では、以下を学ぶことを目的とします。

- SocketCANの基本
- vcan0による仮想CAN通信
- ros2_socketcanによるCANとROS2の接続
- ROS2 C++ノードの作成
- CAN IDによるメッセージ分岐
- CANデータの簡単なデコード
- 車載・組込みシステムに近い通信処理の基礎

---

## 完了条件

最低限、以下を満たせば完了です。

1. `can_ros2_practice` パッケージを作成している
2. `speed_monitor_node` をC++で実装している
3. `/from_can_bus` を購読している
4. CAN ID `0x100` の `data[0]` を車速として読み取っている
5. 車速が60以上ならWARN、60未満ならINFOでログ出力している