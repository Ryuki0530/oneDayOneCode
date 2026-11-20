# EventDispatcher

## 問題概要

イベント名に対応する複数のコールバック関数を管理する
`EventDispatcher` クラスを実装してください。

イベントが発行されると、そのイベントに登録された
すべてのコールバック関数が登録順に実行されます。

## 要件

以下のメンバ関数を持つクラスを実装してください。

### 1. subscribe()

`int subscribe(const std::string& event,
               std::function<void(int)> callback)`

- 指定されたイベントにコールバック関数を登録する。
- 登録ごとに一意な整数IDを発行する。
- 発行したIDを返す。
- 同じイベントに複数の関数を登録できる。

### 2. unsubscribe()

`bool unsubscribe(int id)`

- 指定されたIDの登録を解除する。
- 解除に成功した場合は true を返す。
- 存在しないIDの場合は false を返す。

### 3. emit()

`void emit(const std::string& event, int value)`

- 指定されたイベントに登録された関数を実行する。
- 各関数に value を引数として渡す。
- 実行順序は登録順とする。
- 登録されていないイベントは何もしない。

### 4. listener_count()

`size_t listener_count(const std::string& event) const`

- 指定されたイベントの登録数を返す。
- 存在しないイベントの場合は 0 を返す。

## 実行例

EventDispatcher dispatcher;

int id1 = dispatcher.subscribe("temperature", [](int value) {
    std::cout << "Sensor A: " << value << std::endl;
});

int id2 = dispatcher.subscribe("temperature", [](int value) {
    std::cout << "Sensor B: " << value * 2 << std::endl;
});

dispatcher.emit("temperature", 25);

dispatcher.unsubscribe(id1);

dispatcher.emit("temperature", 30);

std::cout << dispatcher.listener_count("temperature")
          << std::endl;

## 期待される出力

Sensor A: 25
Sensor B: 50
Sensor B: 60
1

## 制約

- C++17以上を使用すること。
- std::function を使用すること。
- std::unordered_map または std::map を使用すること。
- コールバックの実行順序を保証すること。
- グローバル変数は使用しないこと。

## 発展課題（任意）

1. subscribe_once() を実装し、
   一度だけ実行されるイベントを登録できるようにする。

2. emit() の実行中に unsubscribe() が呼ばれても、
   安全に処理できるようにする。

3. emit() の実行中に新しいコールバックが登録された場合、
   そのコールバックは次回の emit() から実行する。

※ マルチスレッド対応は今回の対象外とします。