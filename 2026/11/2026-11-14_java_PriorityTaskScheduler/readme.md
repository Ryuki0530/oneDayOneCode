
# PriorityTaskScheduler

## 概要

複数のタスクを優先度に応じて実行する
簡易タスクスケジューラを実装してください。

Javaの `PriorityQueue` を利用し、
優先度が高いタスクから順番に取り出せるようにします。

## 要件

### 1. Taskクラス

以下のフィールドを持つクラスを作成してください。

- `String name`：タスク名
- `int priority`：優先度（数値が大きいほど高い）
- `long sequence`：登録順序

コンストラクタで各フィールドを初期化してください。

### 2. TaskSchedulerクラス

内部で `PriorityQueue<Task>` を使用し、
以下のメソッドを実装してください。

#### addTask(String name, int priority)

タスクをキューに追加します。

- 同じ優先度の場合、先に登録されたタスクを優先
- sequenceは0から始まる連番とする

#### executeNext()

最も優先度の高いタスクを取り出し、
その名前を返します。

- タスクが存在しない場合は `null` を返す
- 取り出したタスクはキューから削除する

#### peekNext()

次に実行されるタスク名を返します。

- キューから削除しない
- 空の場合は `null` を返す

#### size()

現在登録されているタスク数を返します。

## 実行例

```java
TaskScheduler scheduler = new TaskScheduler();

scheduler.addTask("Logging", 1);
scheduler.addTask("MotorControl", 5);
scheduler.addTask("SensorRead", 3);
scheduler.addTask("EmergencyStop", 10);
scheduler.addTask("MotorUpdate", 5);

System.out.println(scheduler.peekNext());
System.out.println(scheduler.executeNext());
System.out.println(scheduler.executeNext());
System.out.println(scheduler.executeNext());
System.out.println(scheduler.executeNext());
System.out.println(scheduler.executeNext());
System.out.println(scheduler.executeNext());
```

## 期待される出力

```text
EmergencyStop
EmergencyStop
MotorControl
MotorUpdate
SensorRead
Logging
null
```

## 制約

- `PriorityQueue` を必ず使用すること
- タスクのソートに `List.sort()` を使用しないこと
- 同じ優先度ではFIFOを保証すること
- タスク名の重複を許可すること
- 優先度には負数も指定できること

## 発展課題（任意）

1. `cancelTask(String name)` を追加し、
   指定した名前のタスクをキャンセルする。
   同名タスクが複数ある場合は、登録順が
   最も早いものを1件だけ削除する。

2. `TaskScheduler<T>` にジェネリクス化し、
   String以外のタスクデータも扱えるようにする。

3. タスクの実行履歴を保存し、
   `getHistory()` で取得できるようにする。
