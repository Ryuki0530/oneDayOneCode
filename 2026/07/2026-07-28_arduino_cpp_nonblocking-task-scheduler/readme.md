# OneDayOneCode - 2026-07-28

## テーマ

Arduino C++でノンブロッキングタスクスケジューラを実装する

---

## 背景

組込みシステムでは、複数の処理を同時に動作させる必要がある。

例えば、

- LEDを100ms周期で点滅
- センサを500ms周期で読み取る
- シリアル通信を1000ms周期で送信

これらを `delay()` を使って実装すると、他の処理が停止してしまう。

そのため、実際の組込み開発では `millis()` を利用した周期実行や、RTOSによるタスク管理が広く利用されている。

今回はRTOSを使わず、Arduinoだけで簡易的なタスクスケジューラを実装する。

---

## 課題

次のクラスを設計する。

```cpp
class Task
{
public:
    Task(unsigned long interval,
         void (*callback)());

    void update();

private:
    // 必要なメンバは自由に設計してよい
};
```

### 要件

- `delay()` を使用してはいけない
- 時刻管理は `millis()` を使用すること
- `update()` は頻繁に呼ばれることを前提とする
- 実行周期に達したら callback を1回だけ実行すること

---

## 動作確認

最低3個のタスクを登録すること。

### Task1

100msごと

LEDを点灯・消灯する

### Task2

500msごと

シリアルへ

```
Sensor Update
```

を表示する

### Task3

1000msごと

シリアルへ

```
Heartbeat
```

を表示する

---

## 実行例

```
Heartbeat
Sensor Update
Sensor Update
Heartbeat
Sensor Update
...
```

LEDは100ms周期で点滅し続けること。

---

## 制約

- delay禁止
- while待ち禁止
- グローバル変数は必要最小限
- クラスを利用して実装すること

---

## 発展課題

以下のいずれか1つ以上に挑戦する。

### レベル1

Taskを複数保持できるSchedulerクラスを作成する。

```cpp
Scheduler scheduler;

scheduler.addTask(...);
scheduler.update();
```

### レベル2

Taskの開始・停止機能を追加する。

```cpp
task.start();
task.stop();
```

### レベル3

実行回数を指定できるTaskを作る。

例

```
5回だけ実行
```

### レベル4

周期を実行中に変更できるようにする。

```
task.setInterval(200);
```

---

## 学習ポイント

この課題では以下の知識を学ぶ。

- ArduinoにおけるC++
- クラス設計
- コールバック関数
- 関数ポインタ
- millis()による時間管理
- ノンブロッキング設計
- 周期タスク
- RTOSへ繋がる考え方

---

## ゴール

RTOSを使用しなくても、複数の周期処理を同時に動作させられる設計を理解すること。

この設計はROS2のTimerや、車載ECUの周期タスク、PLCなどでも広く利用されている基本的な考え方である。