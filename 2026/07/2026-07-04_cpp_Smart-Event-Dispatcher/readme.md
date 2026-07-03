# OneDayOneCode

## 2026-07-04
## C++
## タイトル
Smart-Event-Dispatcher

---

## 問題

イベントディスパッチャを実装してください。

イベント名に応じて登録された処理を実行できるクラスを作成します。

### 要件

EventDispatcher クラスを作成すること。

以下のメンバ関数を実装してください。

```cpp
void subscribe(const std::string& event_name,
               std::function<void()> callback);

void publish(const std::string& event_name);
```

### 動作

以下のようなコードが動作すること。

```cpp
EventDispatcher dispatcher;

dispatcher.subscribe("start", [](){
    std::cout << "System Start\n";
});

dispatcher.subscribe("start", [](){
    std::cout << "Initialize Sensor\n";
});

dispatcher.subscribe("stop", [](){
    std::cout << "System Stop\n";
});

dispatcher.publish("start");

dispatcher.publish("stop");
```

### 出力例

```
System Start
Initialize Sensor
System Stop
```

---

## 条件

- std::unordered_map を使用すること
- コールバックは std::function<void()> を利用すること
- 同じイベントには複数登録できること
- 未登録イベントを publish() しても何も起こらないこと
- C++17以上

---

## 学べること

- std::function
- ラムダ式
- std::unordered_map
- コールバック設計
- Observer/Event Dispatcher パターンの基礎

---
