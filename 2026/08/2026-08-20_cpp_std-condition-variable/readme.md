# 今日のC++課題：条件変数を使ったスレッドセーフキュー

`std::condition_variable`を使い、複数スレッドから安全に利用できる固定容量のキュー`BlockingQueue`を実装してください。

キューが満杯の場合、`push()`を呼び出したスレッドは空きができるまで待機します。

キューが空の場合、`pop()`を呼び出したスレッドはデータが追加されるまで待機します。

## 実装するクラス

```cpp
class BlockingQueue {
public:
    explicit BlockingQueue(std::size_t capacity);

    void push(int value);
    int pop();

private:
    // 必要なメンバーを追加する
};
```

## 動作条件

### `push(int value)`

- キューに空きがある場合、値を追加する
- キューが満杯の場合、空きができるまで待機する
- 値を追加したら、待機中のスレッドへ通知する

### `pop()`

- キューに値がある場合、先頭の値を取り出して返す
- キューが空の場合、値が追加されるまで待機する
- 値を取り出したら、待機中のスレッドへ通知する

## 制約

- C++17を使用する
- キューの実体には`std::queue<int>`を使用する
- 排他制御には`std::mutex`を使用する
- 待機処理には`std::condition_variable`を使用する
- ビジーウェイトは禁止
- `std::this_thread::sleep_for()`を同期処理の代わりに使用しない
- `push()`と`pop()`では`std::unique_lock`を使用する
- `condition_variable::wait()`には条件判定用のラムダ式を渡す

## スターターコード

```cpp
#include <condition_variable>
#include <cstddef>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

class BlockingQueue {
public:
    explicit BlockingQueue(std::size_t capacity)
        : capacity_(capacity) {
    }

    void push(int value) {
        // TODO:
        // キューに空きができるまで待機し、値を追加する
    }

    int pop() {
        // TODO:
        // キューに値が追加されるまで待機し、先頭の値を取り出す
    }

private:
    std::queue<int> queue_;
    const std::size_t capacity_;

    // TODO:
    // mutexとcondition_variableを追加する
};

void producer(BlockingQueue& queue) {
    for (int value = 1; value <= 10; ++value) {
        queue.push(value);
        std::cout << "Produced: " << value << '\n';
    }
}

void consumer(BlockingQueue& queue) {
    for (int i = 0; i < 10; ++i) {
        const int value = queue.pop();
        std::cout << "Consumed: " << value << '\n';
    }
}

int main() {
    BlockingQueue queue(3);

    std::thread producer_thread(producer, std::ref(queue));
    std::thread consumer_thread(consumer, std::ref(queue));

    producer_thread.join();
    consumer_thread.join();
}
```

## 実行結果の例

```text
Produced: 1
Produced: 2
Produced: 3
Consumed: 1
Consumed: 2
Produced: 4
Consumed: 3
...
Consumed: 10
```

スレッドの実行順序によって表示順は変化します。

ただし、`Consumed`側の値は必ず`1`から`10`まで順番に取り出されるものとします。

## ビルドと実行

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -pthread main.cpp -o main
./main
```

## ヒント

次の2つの条件変数を用意すると実装しやすくなります。

- キューが満杯ではないことを待つ条件変数
- キューが空ではないことを待つ条件変数

`wait()`は次のように使用できます。

```cpp
condition.wait(lock, [&] {
    return /* 待機を終了する条件 */;
});
```

`push()`と`pop()`のどちらで、どちらの条件変数へ通知すべきか考えてみてください。