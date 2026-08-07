# ROS2 Component Performance Practice: Separate Process vs Composed Process

## 目的

ROS 2 Component / Composable Node を使い、
複数ノードを個別プロセスで動かした場合と、
単一プロセス内にまとめた場合の通信性能差を可視化する。

## 学習テーマ

- rclcpp_components
- Composable Node
- component_container
- intra-process communication
- 通信遅延の計測
- CSV出力
- Pythonによるグラフ可視化

## 背景

ROS 2では、通常のノードはそれぞれ別プロセスとして起動できる。

例:

image_source_node プロセス
latency_monitor_node プロセス

この場合、ノード間通信は通常、DDSを経由したプロセス間通信になる。

一方、Componentとしてノードを作ると、
複数ノードを1つのcomponent_container内に読み込んで実行できる。

例:

component_container プロセス
├── ImageSource Component
└── LatencyMonitor Component

さらに intra-process communication を有効にすると、
同一プロセス内のPublisher / Subscriber間通信を効率化できる。

## 今日作るもの

パッケージ名:

component_perf_practice

## 作成するComponent

### 1. ImageSource

ノード名:

image_source

役割:

疑似画像データを高速にpublishする。

トピック:

/dummy_image

型:

sensor_msgs/msg/Image

仕様:

- 画像サイズは 640 x 480
- encoding は mono8
- data は 640 * 480 byte の配列
- publish周期は 10ms
- つまり約100Hzでpublishする
- header.stamp に publish 時刻を入れる

### 2. LatencyMonitor

ノード名:

latency_monitor

役割:

/dummy_image をsubscribeし、通信遅延を計測する。

仕様:

- 受信時刻 now と msg.header.stamp の差を計算する
- 遅延を millisecond 単位で扱う
- 1秒ごとに以下をログ出力する

表示内容:

- 受信メッセージ数
- 平均遅延
- 最大遅延
- 最小遅延

さらに、計測結果をCSVファイルに保存する。

CSV形式:

elapsed_sec,received_count,avg_latency_ms,min_latency_ms,max_latency_ms

## 比較する実行条件

### 条件A: 個別プロセス実行

ImageSource と LatencyMonitor を別々の ros2 run で起動する。

ターミナル1:

ros2 run component_perf_practice image_source_node

ターミナル2:

ros2 run component_perf_practice latency_monitor_node --ros-args -p output_csv:=separate.csv

この条件では、PublisherとSubscriberが別プロセスで動く。

### 条件B: 単一プロセス実行

ComposableNodeContainer を使い、
ImageSource と LatencyMonitor を同じプロセス内で起動する。

実行コマンド:

ros2 launch component_perf_practice composed_perf.launch.py output_csv:=composed.csv

launchファイルでは、両Componentに以下を指定する。

extra_arguments=[
    {'use_intra_process_comms': True}
]

この条件では、PublisherとSubscriberが同じプロセス内で動く。

## 可視化

Pythonスクリプト plot_result.py を作成し、
separate.csv と composed.csv を読み込んでグラフ化する。

出力画像:

latency_result.png

グラフに表示するもの:

- 横軸: elapsed_sec
- 縦軸: avg_latency_ms
- separate.csv の平均遅延
- composed.csv の平均遅延

余裕があれば、最大遅延 max_latency_ms も別グラフで表示する。

## 必須条件

- C++でComponentを実装すること
- main関数はComponent本体には書かないこと
- RCLCPP_COMPONENTS_REGISTER_NODE を使うこと
- CMakeLists.txt では add_library を使うこと
- rclcpp_components_register_node を使って、ros2 run でも起動できるようにすること
- launchファイルで ComposableNodeContainer を使うこと
- 計測結果をCSVに保存すること
- Pythonでグラフ画像を生成すること

## 考察

以下について短くまとめる。

1. 個別プロセス実行と単一プロセス実行で、平均遅延はどう変化したか
2. 最大遅延は安定したか、不安定だったか
3. Component化によって通信が速くなる理由は何か
4. 逆に、単一プロセス化のデメリットは何か
5. 車載・ロボット開発で、どのようなノードをComponent化すると効果がありそうか