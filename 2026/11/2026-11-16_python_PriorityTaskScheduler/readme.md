
# PriorityTaskScheduler

## 問題概要

複数のタスクを優先度に応じて処理する
`PriorityTaskScheduler` クラスを実装してください。

タスクには以下の情報があります。

- name: タスク名（文字列）
- priority: 優先度（整数、小さいほど優先度が高い）
- 登録順序: 同じ優先度の場合、先に登録されたタスクを優先する

## 要件

### 1. add_task(name, priority)

新しいタスクを登録します。

- name: str
- priority: int

### 2. execute_next()

最も優先度の高いタスクを取り出し、
タスク名を返します。

- 同じ優先度なら登録順に処理する
- タスクが存在しなければ None を返す
- 実行したタスクはキューから削除する

### 3. pending_count()

未処理のタスク数を返します。

### 4. peek_next()

次に実行されるタスク名を返します。

- タスクは削除しない
- タスクが存在しなければ None を返す

## 実行例

scheduler = PriorityTaskScheduler()

scheduler.add_task("Logging", 3)
scheduler.add_task("EmergencyStop", 1)
scheduler.add_task("SensorUpdate", 2)
scheduler.add_task("MotorControl", 1)

print(scheduler.peek_next())
# EmergencyStop

print(scheduler.execute_next())
# EmergencyStop

print(scheduler.execute_next())
# MotorControl

print(scheduler.pending_count())
# 2

print(scheduler.execute_next())
# SensorUpdate

print(scheduler.execute_next())
# Logging

print(scheduler.execute_next())
# None

## 制約

- heapq モジュールを使用すること
- タスクの登録順序を保持すること
- タスク数は最大100,000件
- add_task と execute_next は O(log N) を目標とする
- peek_next は O(1) を目標とする

## 発展課題（任意）

cancel_task(name) を追加してください。

- 指定した名前の未処理タスクを取り消す
- 同じ名前のタスクが複数ある場合はすべて取り消す
- 該当するタスクがなければ False を返す
- 1件以上取り消した場合は True を返す
- 取り消したタスクは execute_next で返さない
- pending_count は取り消し済みタスクを除外する
