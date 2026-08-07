# 今日のC++課題：std::exchangeで状態を更新する

## 目的
`std::exchange` の使い方を覚える。

## 問題
整数の状態を保持する `State` クラスを作成してください。

`State` クラスに以下を実装します。

- コンストラクタで初期値を受け取る
- `update(int new_value)` を呼ぶと値を更新する
- `update()` は「更新前の値」を返す
- `get()` で現在値を取得できる

ただし `update()` の値の更新には
`std::exchange` を使用してください。

## 実行例

State state(10);

std::cout << state.update(20) << '\n';
std::cout << state.get() << '\n';

## 出力

10
20

## ヒント

std::exchange(変数, 新しい値)

は、

1. 変数の古い値を取得
2. 変数に新しい値を代入
3. 古い値を返す

という処理を一度に行います。

`std::exchange` を使うには `<utility>` が必要です。

## コンパイル

g++ -std=c++17 -Wall -Wextra main.cpp -o main