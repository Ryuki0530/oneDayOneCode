# 今日のC++課題：終了可能なスレッドセーフキュー

## 概要

`std::mutex` と `std::condition_variable` を使用して、複数のスレッドから安全に利用できるキューを実装してください。

今回は、キューを単にスレッドセーフにするだけでなく、処理の終了を通知する `close()` 機能も実装します。

## 使用する規格

C++17

## 実装するクラス

次のインターフェースを持つ `BlockingQueue` クラスを実装してください。

    template <typename T>
    class BlockingQueue {
    public:
        bool push(T value);
        std::optional<T> pop();
        void close();

    private:
        // 必要なメンバ変数を定義する
    };

## 各関数の仕様

### push(T value)

- キューに値を追加する
- 追加後、`pop()` で待機しているスレッドを起こす
- キューが `close()` 済みの場合は追加せず、`false` を返す
- 追加に成功した場合は `true` を返す

### pop()

- キューに要素があれば、先頭要素を取り出して返す
- キューが空の場合は、要素が追加されるか `close()` されるまで待機する
- `close()` 済みでも、キューに残っている要素は取り出す
- `close()` 済みかつキューが空の場合は `std::nullopt` を返す

### close()

- キューを終了状態にする
- `pop()` で待機しているすべてのスレッドを起こす
- 複数回呼び出されても問題なく動作すること

## 動作確認

`main()` では以下を行ってください。

1. `BlockingQueue<int>` を作成する
2. コンシューマースレッドを2本起動する
3. 各コンシューマーは `pop()` を繰り返し、取得した値を表示する
4. `pop()` が `std::nullopt` を返したらループを終了する
5. メインスレッドから `1` ～ `10` を `push()` する
6. すべて追加した後に `close()` を呼び出す
7. コンシューマースレッドを `join()` する
8. `close()` 後の `push(100)` が `false` を返すことを確認する

## 使用する主なヘッダー

    <condition_variable>
    <iostream>
    <mutex>
    <optional>
    <queue>
    <thread>
    <utility>

## 制約

- キュー本体には `std::queue<T>` を使用する
- ビジーウェイトは禁止
- `pop()` の待機には `std::condition_variable::wait()` を使用する
- 共有状態へのアクセスはすべてミューテックスで保護する
- デッドロックを発生させないこと

## コンパイル例

    g++ -std=c++17 -Wall -Wextra -pthread main.cpp -o main
    ./main

## 考察ポイント

`pop()` の待機条件は、単に「キューが空ではない」だけでは不十分です。

キューが空のまま `close()` された場合にも、待機中のスレッドが処理を終了できる条件を考えてください。