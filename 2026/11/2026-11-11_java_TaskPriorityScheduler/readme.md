
# TaskPriorityScheduler

## 問題

複数のタスクを優先度に従って処理する
簡易タスクスケジューラを実装してください。

各タスクは以下の情報を持ちます。

- name：タスク名（String）
- priority：優先度（int、値が小さいほど優先）
- arrivalOrder：登録順序（int）

タスクは優先度が高い順に実行します。
優先度が同じ場合は、先に登録されたタスクを
優先してください。

## 実装するクラス

### Task
タスクの情報を保持するクラス。

### TaskScheduler
以下のメソッドを実装してください。

- addTask(String name, int priority)
  - タスクを登録する。
- executeNext()
  - 最優先のタスク名を返し、キューから削除する。
  - タスクが存在しない場合は null を返す。
- pendingCount()
  - 未実行タスク数を返す。

## 入出力例

登録順：
addTask("Logging", 3)
addTask("SensorRead", 1)
addTask("MotorControl", 1)
addTask("Backup", 5)

実行結果：
SensorRead
MotorControl
Logging
Backup

## 制約

- PriorityQueueを使用すること。
- Comparatorを用いて優先順位を定義すること。
- 同じ優先度ではFIFOを保証すること。
- タスク名の重複を許可すること。
- 優先度は任意のint値を許可すること。

## 発展課題（任意）

- cancelTask(String name)を追加する。
- 指定した名前の未実行タスクをすべて削除する。
- 削除した件数を返す。

## 学習目標

- PriorityQueueの利用
- Comparatorによる複数条件の比較
- オブジェクトの管理
- クラス設計とカプセル化
