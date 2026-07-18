# OneDayOneCode

## 2026-07-18
### C++ - Event Dispatcher

## 問題

イベントを登録・発行できる簡易イベントディスパッチャを実装してください。

以下のイベントを扱います。

- CONNECT
- DISCONNECT
- ERROR

イベントには文字列メッセージが付属します。

### 要件

次のクラスを作成してください。

```cpp
class EventDispatcher
```

### メンバ関数

```cpp
void subscribe(EventType type,
               std::function<void(const std::string&)> callback);

void publish(EventType type,
             const std::string& message);
```

### 動作例

```cpp
dispatcher.subscribe(EventType::CONNECT,
    [](const std::string& msg){
        std::cout << "Connected : " << msg << std::endl;
    });

dispatcher.subscribe(EventType::ERROR,
    [](const std::string& msg){
        std::cout << "Error : " << msg << std::endl;
    });

dispatcher.publish(EventType::CONNECT, "Robot01");

dispatcher.publish(EventType::ERROR,
                   "Joint timeout");
```

出力

```
Connected : Robot01
Error : Joint timeout
```

---

## 制約

内部実装は自由ですが、

- enum class
- std::unordered_map
- std::vector
- std::function
- ラムダ式

を活用してください。

複数のコールバックが登録されている場合は、
登録順にすべて呼び出してください。

登録されていないイベントを publish しても
何も起こらないようにしてください。

---

## 発展課題

余裕があれば

```cpp
unsubscribe()
```

も実装してください。

subscribe() が登録IDを返し、
そのIDを使って解除できるようにしてください。

---

## 学べること

- enum class
- std::function
- ラムダ式
- コールバック設計
- Dispatcherパターン
- Publisher / Subscriberの基本構造

ROS2のTopic通信やGUIライブラリ(Qt)のSignal/Slot、
ゲームエンジンのイベント処理などで広く使われる設計です。