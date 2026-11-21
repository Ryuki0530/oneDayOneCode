
# EventDispatcher

## 問題概要

イベント名に対して複数のコールバック関数を登録し、
イベント発生時に対応する関数を実行する
`EventDispatcher` クラスを実装してください。

GUIアプリケーションやロボット制御システムなどで利用される
イベント駆動型プログラミングの基本構造を学びます。

## 実装するクラス

class EventDispatcher:

    def subscribe(self, event_name: str, callback) -> None:
        pass

    def unsubscribe(self, event_name: str, callback) -> bool:
        pass

    def emit(self, event_name: str, data=None) -> int:
        pass

    def listener_count(self, event_name: str) -> int:
        pass

## 仕様

### 1. subscribe(event_name, callback)

指定したイベントにコールバック関数を登録します。

- 同じイベントに複数の関数を登録できます。
- 登録した順序を保持してください。
- 同じ関数を同じイベントに重複登録することはできません。
- 重複登録された場合は何もしません。

### 2. unsubscribe(event_name, callback)

指定したイベントからコールバック関数を解除します。

- 解除に成功した場合は True を返します。
- 登録されていない場合は False を返します。

### 3. emit(event_name, data=None)

指定したイベントを発行します。

- 登録されたコールバック関数を登録順に実行します。
- 各関数には data を引数として渡します。
- 戻り値は実行したコールバック関数の数です。
- 登録されていないイベントの場合は 0 を返します。

### 4. listener_count(event_name)

指定したイベントに登録されている関数の数を返します。

## 実行例
```
def on_temperature(data):
    print(f"Temperature: {data}")

def on_warning(data):
    if data > 80:
        print("WARNING!")

dispatcher = EventDispatcher()

dispatcher.subscribe("sensor", on_temperature)
dispatcher.subscribe("sensor", on_warning)

print(dispatcher.listener_count("sensor"))
# 2

print(dispatcher.emit("sensor", 85))
# Temperature: 85
# WARNING!
# 2

dispatcher.unsubscribe("sensor", on_warning)

print(dispatcher.emit("sensor", 60))
# Temperature: 60
# 1
```
## 制約条件

- 外部ライブラリは禁止です。
- dict、list、setなどの標準データ構造は使用可能です。
- コールバック関数の戻り値は無視してください。
- コールバック関数は例外を発生させないものとします。

## 発展課題（任意）

### 1. 一度だけ実行されるイベント

once(event_name, callback)

- 一度だけ実行されるコールバックを登録します。
- 実行後は自動的に登録を解除してください。

### 2. イベント発行中の登録変更

コールバック関数の実行中に subscribe() や
unsubscribe() が呼ばれても安全に動作するようにしてください。

- emit() 開始時点の登録内容を実行対象とします。
- 実行中の変更は次回の emit() から反映してください。

## ファイル名

main.py

## 推奨ディレクトリ名

2026-11-21_python_EventDispatcher
