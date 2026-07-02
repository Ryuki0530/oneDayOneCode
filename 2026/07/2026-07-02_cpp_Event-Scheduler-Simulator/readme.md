# OneDayOneCode

## 2026-07-02
# C++ Event Scheduler Simulator

## 目的

イベント駆動型プログラムの設計を学ぶ。

---

## 問題

イベントスケジューラを作成してください。

イベントには

- 実行時刻(ms)
- イベント名
- コールバック関数

があります。

開始時刻は0msとします。

イベントは時刻順に実行してください。

---

## 動作例

登録

1000 START_SENSOR
500  LOG
1500 STOP_SENSOR
800  CHECK_STATUS

↓

実行結果

500ms : LOG
800ms : CHECK_STATUS
1000ms : START_SENSOR
1500ms : STOP_SENSOR

---

## 要件

### クラス Event

保持するもの

- 実行時刻
- 名前
- callback(std::function<void()>)

---

### クラス Scheduler

最低でも

addEvent()

run()

を持つこと。

---

### run()

時刻順にイベントを処理する。

std::this_thread::sleep_for() を使って
実際の時間経過を再現してもよい。

---

## 発展課題1

イベント実行中に

新しいイベントを追加できるようにする。

例

1000ms

↓

callback内で

1200msのイベント追加

---

## 発展課題2

イベントキャンセル機能

cancelEvent(name)

---

## 発展課題3

同じ時刻のイベントは

登録順

で実行する。

---

## 使用推奨ライブラリ

- <queue>
- <functional>
- <chrono>
- <thread>
- <vector>
- <memory>

---

## 学べること

このような考え方は

- ROS2 Executor
- GUIイベントループ
- ECUタスクスケジューラ
- ゲームエンジン
- 非同期処理

などで広く使われています。