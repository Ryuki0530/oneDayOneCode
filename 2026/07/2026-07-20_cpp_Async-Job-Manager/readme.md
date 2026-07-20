# OneDayOneCode

## Title
Async Job Manager

## Language
C++17

## Difficulty
★★★★☆

## Problem

`std::async`を利用して、複数の非同期ジョブを並列実行するプログラムを作成してください。

各ジョブは一定時間待機した後、計算結果を返します。

## Requirements

1. `std::async`を使用して、3つのジョブを実行する。
2. 各ジョブには、1から3までのIDを割り当てる。
3. 各ジョブは、100ミリ秒から500ミリ秒のランダムな時間だけ待機する。
4. 待機後、ジョブIDの2乗を計算して返す。
5. メイン処理では、各ジョブの結果を`std::future`から取得する。
6. 全ジョブの結果と、その合計値を表示する。

## Expected Output

実行ごとに待機時間やジョブの終了順序が変わっても構いません。

出力例：

Job1 = 1
Job2 = 4
Job3 = 9

Total = 14

## Constraints

- `std::thread`は使用しないこと。
- 非同期実行には`std::async`を使用すること。
- 非同期処理の戻り値は`std::future`で受け取ること。
- 並列実行を保証するため、`std::launch::async`を指定すること。
- 待機処理には`std::this_thread::sleep_for`を使用してよい。
- C++17以上でコンパイルすること。

## Advanced Challenge

Job2だけ、20%の確率で例外を送出するように変更してください。

送出する例外：

throw std::runtime_error("Job2 failed");

`std::future::get()`を呼び出した際に例外を受け取り、次のように表示してください。

Job1 = 1
Job2 failed
Job3 = 9

他のジョブで発生した正常な結果は、そのまま表示してください。

例外が発生したジョブの結果は、合計値に含めないものとします。

## Learning Objectives

- `std::async`による非同期処理
- `std::future`による戻り値の取得
- `std::launch::async`の役割
- 非同期処理からの例外伝播
- 複数の非同期タスクの管理

## Build Example

g++ -std=c++17 -Wall -Wextra -pthread main.cpp -o async_job_manager

## Run Example

./async_job_manager