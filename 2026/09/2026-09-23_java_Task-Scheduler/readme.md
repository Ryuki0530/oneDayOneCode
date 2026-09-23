# TaskScheduler

## 概要

タスクを優先度順に処理する簡単なスケジューラを作成してください。

各タスクは以下の情報を持ちます。

- name : タスク名
- priority : 優先度（数値が大きいほど優先）
- order : 登録された順番

`PriorityQueue` を使用して、次のルールでタスクを取り出してください。

1. priority が高いタスクを先に処理する
2. priority が同じ場合は、先に登録されたタスクを処理する

---

## 実装するクラス

### Task

以下のフィールドを持つクラスを作成してください。

- String name
- int priority
- int order

---

### TaskScheduler

以下のメソッドを実装してください。

void addTask(String name, int priority)

タスクを追加します。

Task pollTask()

最も優先度の高いタスクを1つ取り出します。

タスクが存在しない場合は null を返してください。

---

## 実行例

以下の順番でタスクを追加します。

Compile  priority=2
Test     priority=3
Deploy   priority=1
Lint     priority=3
Package  priority=2

すべてのタスクを取り出して表示すると、

Test
Lint
Compile
Package
Deploy

となるようにしてください。

---

## 条件

- java.util.PriorityQueue を使用すること
- Comparator を使用して並び順を指定すること
- Collections.sort や List.sort を使ってはいけない
- priority が同じ場合の登録順を保持すること

---

## Bonus

Task peekTask()

を追加して、

「次に処理されるタスクを削除せずに取得する」

機能も実装してください。