# 今日のC++課題：`std::call_once` で一度だけ初期化する

## 目的

複数スレッドから同時に呼ばれても、初期化処理が1回だけ実行される仕組みを作成してください。

## 条件

C++17を使用してください。

次のクラスを作成します。

`Database`

### メンバ関数

`void initialize()`

- `std::call_once` を使用する
- 初回だけ以下を表示する

    Database initialized.

- 2回目以降の呼び出しでは何もしない

`void query(int id)`

- 最初に `initialize()` を呼ぶ
- その後、以下の形式で表示する

    Query: <id>

## main関数

5個のスレッドを生成し、それぞれ同じ `Database` インスタンスに対して

    db.query(i);

を実行してください。

## 期待する性質

`Query:` は5回表示されますが、

    Database initialized.

は必ず1回だけ表示されること。

## 制約

- `std::once_flag` を使用すること
- `std::call_once` を使用すること
- 自分でbool変数を使って初期化済みか判定しないこと

## コンパイル例

    g++ -std=c++17 -Wall -Wextra -pthread main.cpp -o main