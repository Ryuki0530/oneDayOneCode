# 今日のC++課題：バックグラウンド計算

## 使用規格

C++17

## 課題

`std::packaged_task`、`std::future`、`std::thread`を使用して、時間のかかる計算を別スレッドで実行してください。

次の関数を用意します。

int calculate(int value);

この関数は以下の処理を行います。

1. 1秒間待機する
2. `value * value` を返す

`main`関数では以下の手順で処理してください。

1. `calculate`を登録した`std::packaged_task<int(int)>`を作成する
2. `get_future()`で結果を受け取るための`std::future<int>`を取得する
3. `std::thread`を使い、引数`12`でタスクを実行する
4. 計算中に`Calculating...`と表示する
5. `future.get()`で計算結果を取得する
6. `Result: 144`と表示する
7. スレッドを安全に終了させる

## 実行例

Calculating...
Result: 144

## 制約

- C++17でコンパイルすること
- `std::async`は使用しないこと
- スレッドの終了時に`join()`を呼び出すこと

## コンパイル例

g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main

std::packaged_taskは、関数やラムダ式を「あとで実行できるタスク」として包む仕組みです。戻り値は直接受け取らず、対応するstd::futureを通して取得します。

イメージは次のとおりです。

``` 
std::packaged_task<int(int)> task(関数);
std::future<int> result = task.get_future();
```