# 今日のC++課題：固定長リングバッファの実装

## 概要

固定された容量を持つリングバッファ（循環バッファ）を、クラステンプレートとして実装してください。

リングバッファは末尾まで到達すると、次の格納位置が先頭に戻るデータ構造です。

今回は容量を超えて `push()` した場合、最も古い要素を上書きする仕様にします。

## 使用する規格

C++17

## 実装するクラス

    template <typename T, std::size_t Capacity>
    class RingBuffer {
    public:
        void push(T value);
        std::optional<T> pop();

        const T& front() const;
        const T& back() const;

        bool empty() const;
        bool full() const;
        std::size_t size() const;

    private:
        std::array<std::optional<T>, Capacity> data_;
        std::size_t head_ = 0;
        std::size_t size_ = 0;
    };

## 各関数の仕様

### push(T value)

- 新しい値をリングバッファの末尾に追加する
- 空きがある場合は通常どおり追加する
- バッファが満杯の場合は、最も古い要素を上書きする
- 値の格納にはムーブを利用すること

### pop()

- 最も古い要素を取り出して返す
- 取り出した格納位置を `std::nullopt` に戻す
- バッファが空の場合は `std::nullopt` を返す

### front()

- 最も古い要素へのconst参照を返す
- バッファが空の場合は `std::out_of_range` を送出する

### back()

- 最も新しい要素へのconst参照を返す
- バッファが空の場合は `std::out_of_range` を送出する

### empty()

バッファが空なら `true` を返す。

### full()

バッファが満杯なら `true` を返す。

### size()

現在格納されている要素数を返す。

## 動作確認

次の処理を `main()` に実装してください。

1. 容量3の `RingBuffer<std::string>` を作成する
2. `"A"`、`"B"`、`"C"` を追加する
3. `front()` が `"A"`、`back()` が `"C"` であることを確認する
4. `"D"` を追加する
5. 最も古い `"A"` が上書きされたことを確認する
6. `pop()` を繰り返し、次の順番で表示されることを確認する

    B
    C
    D

7. 空になった後の `pop()` が `std::nullopt` を返すことを確認する
8. 空の状態で `front()` を呼び出し、例外を捕捉する

## 使用する主なヘッダー

    <array>
    <cstddef>
    <iostream>
    <optional>
    <stdexcept>
    <string>
    <utility>

## 制約

- 動的メモリ確保を直接使用しない
- `std::vector` や `std::deque` は使用しない
- 要素の格納には `std::array<std::optional<T>, Capacity>` を使用する
- インデックスの循環には剰余演算 `%` を使用する
- `Capacity` が0の場合はコンパイルエラーにする

## ヒント

新しい要素を書き込む位置は、次の式で求められます。

    (head_ + size_) % Capacity

満杯の状態で上書きした場合は、最も古い要素を示す `head_` も次の位置へ進める必要があります。

    head_ = (head_ + 1) % Capacity

`back()` が参照する位置は、単純な `head_ + size_ - 1` では配列の範囲を超える可能性があるため、循環を考慮してください。

## コンパイル例

    g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
    ./main