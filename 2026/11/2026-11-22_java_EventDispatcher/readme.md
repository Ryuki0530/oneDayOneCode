
# EventDispatcher

## 概要

イベントの種類に応じて、登録されたリスナーへ通知を行う
イベント管理システム `EventDispatcher` を実装してください。

イベント駆動型プログラミングの基本となる
Observerパターンを意識した設計とします。

## 要件

### 1. EventListenerインターフェース

以下のメソッドを持つインターフェースを定義してください。

```java
void onEvent(String eventType, String message);
```

### 2. EventDispatcherクラス

以下のメソッドを実装してください。

#### subscribe(String eventType, EventListener listener)

- 指定されたイベントにリスナーを登録する。
- 同じイベントに複数のリスナーを登録できる。
- 同じリスナーインスタンスの重複登録は許可しない。
- リスナーの登録順序を保持する。

#### unsubscribe(String eventType, EventListener listener)

- 指定されたイベントからリスナーを解除する。
- 登録されていない場合は何もしない。

#### publish(String eventType, String message)

- 指定されたイベントに登録されている全リスナーへ通知する。
- 通知は登録順序で実行する。
- 登録されていないイベントの場合は何もしない。

#### listenerCount(String eventType)

- 指定されたイベントのリスナー数を返す。
- イベントが存在しない場合は0を返す。

## 実装条件

- `Map` と `List` を使用すること。
- `EventListener` はラムダ式で実装可能にすること。
- リスナーの重複判定はインスタンスの同一性（==）で行うこと。
- 外部ライブラリは使用しない。
- スレッドセーフ性は考慮しなくてよい。

## 実行例

```java
EventDispatcher dispatcher = new EventDispatcher();

EventListener listenerA = (type, message) ->
    System.out.println("A: " + message);

EventListener listenerB = (type, message) ->
    System.out.println("B: " + message);

dispatcher.subscribe("ALERT", listenerA);
dispatcher.subscribe("ALERT", listenerB);
dispatcher.subscribe("ALERT", listenerA);

System.out.println(dispatcher.listenerCount("ALERT"));

dispatcher.publish("ALERT", "Temperature high");

dispatcher.unsubscribe("ALERT", listenerA);

dispatcher.publish("ALERT", "Temperature normal");
```

### 期待される出力

```text
2
A: Temperature high
B: Temperature high
B: Temperature normal
```

## 発展課題（任意）

通知処理中にリスナーが自身を解除しても、
他のリスナーへの通知が正常に継続されるようにしてください。

例：

```java
EventListener[] self = new EventListener[1];

self[0] = (type, message) -> {
    System.out.println("One-shot: " + message);
    dispatcher.unsubscribe(type, self[0]);
};

dispatcher.subscribe("ALERT", self[0]);
```

通知中のリスナー一覧の変更によって
`ConcurrentModificationException` が発生しないように
実装してください。
