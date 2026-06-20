# VCAN Frame Monitor

## 目的

Ubuntu の `vcan0` を使って、仮想CAN通信を体験する。

CANフレームを受信し、CAN ID ごとに以下の情報を集計する監視ツールを作成する。

- 受信回数
- 最新データ
- 前回受信からの経過時間
- 平均周期
- 受信レート

## 使用環境

- Ubuntu
- Python 3
- vcan0
- can-utils
- python-can

## 事前準備

### 1. can-utils のインストール

```bash
sudo apt update
sudo apt install can-utils
```

### 2. Pythonライブラリのインストール

```bash
pip install python-can
```

### 3. vcan0 の作成

```bash
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0
```

確認：

```bash
ip link show vcan0
```

## 課題内容

`vcan_monitor.py` を作成し、`vcan0` からCANフレームを受信して表示する。

## 実装要件

### 必須要件

1. `vcan0` からCANフレームを受信する
2. CAN ID ごとに受信情報を管理する
3. 各CAN IDについて以下を表示する
   - CAN ID
   - 受信回数
   - 最新データ
   - 前回受信からの経過時間
   - 平均周期
4. Ctrl+C で安全に終了する

## 動作確認

別ターミナルで以下を実行する。

```bash
cansend vcan0 100#11223344
cansend vcan0 200#AABBCCDD
cansend vcan0 100#55667788
```

周期送信する場合：

```bash
while true; do cansend vcan0 100#11223344; sleep 0.5; done
```

別IDも送る：

```bash
while true; do cansend vcan0 200#AABBCCDD; sleep 1.0; done
```

## 実行例

```bash
python3 vcan_monitor.py
```

表示例：

```text
CAN ID: 0x100 | count: 12 | latest: 11 22 33 44 | dt: 0.501s | avg: 0.500s | rate: 2.00 Hz
CAN ID: 0x200 | count:  6 | latest: AA BB CC DD | dt: 1.002s | avg: 1.001s | rate: 1.00 Hz
```

## 発展課題

余裕があれば以下を追加する。

1. 受信レート Hz を表示する
2. 一定時間受信がないCAN IDに `TIMEOUT` と表示する
3. `--interface vcan0` のようにコマンドライン引数でIF名を指定できるようにする
4. CSVログ保存機能を追加する
5. CAN ID 0x100 を車速、0x200 をステア角としてデコードする

## ねらい

この課題では、CAN通信における以下を確認する。

- CAN ID ごとに意味を分ける考え方
- 周期送信される車載信号の監視
- 受信周期のズレや停止検出
- ログ解析ツールの基本構造