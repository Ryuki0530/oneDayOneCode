# 今日のC++課題：`std::variant`を使ったイベント処理

## 概要

ロボットから次の3種類のイベントが送られてくるシステムを作成してください。

- 関節角度の更新
- エラーメッセージ
- 緊急停止

それぞれのイベントを `std::variant` に格納し、`std::visit` を使って種類ごとに処理します。

## 使用する構造体

次の3つの構造体を定義してください。

~~~cpp
struct JointPosition {
    std::string joint_name;
    double position;
};

struct ErrorMessage {
    int error_code;
    std::string message;
};

struct EmergencyStop {
    bool active;
};
~~~

## 要件

1. 3種類の構造体を格納できる、次の型を定義してください。

~~~cpp
using RobotEvent = std::variant<
    JointPosition,
    ErrorMessage,
    EmergencyStop
>;
~~~

2. `RobotEvent` を受け取る、次の関数を実装してください。

~~~cpp
void processEvent(const RobotEvent& event);
~~~

3. `processEvent()` では `std::visit` を使用してください。

4. イベントの種類に応じて、次の形式で表示してください。

~~~text
Joint shoulder position: 1.25 rad
Error 404: Motor not found
Emergency stop: ACTIVE
~~~

5. `main()` で `std::vector<RobotEvent>` を作成し、最低でも各種類のイベントを1つずつ格納してください。

6. 範囲for文を使って、すべてのイベントを `processEvent()` に渡してください。

## 実行例

~~~text
Joint shoulder position: 1.25 rad
Joint elbow position: -0.50 rad
Error 404: Motor not found
Emergency stop: ACTIVE
~~~

## 制約

- C++17を使用すること
- `void*` や手動の型番号は使用しないこと
- `std::get` で型を順番に確認するのではなく、`std::visit` を使用すること

## ビルドコマンド

~~~bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
~~~

## 発展課題

時間があれば、イベント処理を行うラムダをまとめるために、次の `Overloaded` を使用してください。

~~~cpp
template<class... Ts>
struct Overloaded : Ts... {
    using Ts::operator()...;
};

template<class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;
~~~