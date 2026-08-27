# 今日のC++課題：`std::unique_lock` でロック範囲を制御する

## 問題

以下の `MessageStore` クラスを実装してください。

複数スレッドから安全にメッセージを追加できるようにします。

ただし、`printLastMessage()` では

1. mutexをロック
2. 最新のメッセージをコピー
3. mutexをアンロック
4. `std::cout` で表示

という流れにしてください。

**表示処理中までmutexを保持しないこと**がポイントです。

## 条件

- C++17を使用する
- `std::mutex` を使用する
- `std::unique_lock<std::mutex>` を使用する
- `unlock()` を明示的に使用する
- `messages_` が空の場合は `"No messages"` と表示する
- `add()` は複数スレッドから呼ばれても安全にする

## 雛形

```cpp
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class MessageStore {
public:
    void add(const std::string& message) {
        // TODO
    }

    void printLastMessage() {
        // TODO
    }

private:
    std::vector<std::string> messages_;
    std::mutex mutex_;
};

int main() {
    MessageStore store;

    std::thread t1([&]() {
        store.add("Hello");
    });

    std::thread t2([&]() {
        store.add("World");
    });

    t1.join();
    t2.join();

    store.printLastMessage();

    return 0;
}
```

## ビルド

```bash
g++ -std=c++17 -Wall -Wextra -pthread main.cpp -o main
```

## 考えるポイント

`std::lock_guard` ではなく `std::unique_lock` を使うことで、

```cpp
lock.unlock();
```

のようにスコープの途中でmutexを解放できます。

なぜ「データをコピーしてからmutexを解除し、その後で表示する」方がよいのかも考えてみてください。