# OneDayOneCode - 2026-07-19

# タイトル
Producer Consumer Queue

## 問題

スレッドセーフなキューを作成してください。

以下の仕様を満たすこと。

### クラス

ThreadSafeQueue

### メンバ関数

void push(int value)
- キューへ値を追加する。

int pop()
- データが無ければ待機する。
- データが追加されたら取り出して返す。

### 条件

- std::mutex
- std::condition_variable
- std::queue

を利用すること。

---

## 動作

Producerスレッド

1〜20までを100msごとにpushする。

Consumerスレッド

20個取り出し、その都度表示する。

表示例

Produced: 1
Consumed: 1
Produced: 2
Consumed: 2
...
Produced: 20
Consumed: 20

---

## 発展課題

Consumerを2スレッドに増やす。

例

Consumer1: 1
Consumer2: 2
Consumer1: 3
Consumer2: 4

のように20個すべてが一度だけ処理されれば成功。

---

## 使用できるライブラリ

<thread>
<mutex>
<condition_variable>
<queue>
<chrono>
<iostream>

---

## 学べること

- std::thread
- std::mutex
- std::unique_lock
- std::condition_variable
- wait()
- notify_one()
- notify_all()
- Producer / Consumer パターン