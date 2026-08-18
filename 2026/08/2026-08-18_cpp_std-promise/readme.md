# 今日のC++課題：`std::promise`で別スレッドの結果を受け取る

## 概要

別スレッドで計算した結果を、`std::promise`と`std::future`を使ってメインスレッドへ渡してください。

## 要件

1. 次の関数を作成すること。

   `void calculateSum(int begin, int end, std::promise<long long> result)`

2. `calculateSum`は、`begin`から`end`までの整数の合計を計算する。

3. 計算結果は戻り値ではなく、`std::promise::set_value()`で渡す。

4. `main`では次の処理を行う。

   - `std::promise<long long>`を作成する
   - `get_future()`で対応する`std::future`を取得する
   - `calculateSum(1, 1000000, ...)`を別スレッドで実行する
   - `future.get()`で結果を受け取る
   - 結果を表示する
   - スレッドを`join()`する

## 期待される出力例

    計算中...
    合計: 500000500000

## 発展課題

`begin > end`の場合は、`std::promise::set_exception()`を使って例外をメインスレッドへ伝えてください。

メインスレッドでは`future.get()`を`try-catch`で囲み、エラーメッセージを表示してください。

## 制約

- C++17を使用すること
- グローバル変数は使用しないこと
- 計算結果を共有変数に直接書き込まないこと

## ビルド例

    g++ -std=c++17 -Wall -Wextra -pedantic -pthread main.cpp -o main