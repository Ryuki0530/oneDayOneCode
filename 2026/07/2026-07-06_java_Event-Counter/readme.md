# OneDayOneCode

## タイトル
EventCounter

## 問題概要

イベントログを読み取り、イベント種別ごとの発生回数を集計してください。

入力は1行につき1イベントです。

### 入力例

LOGIN
ERROR
LOGIN
LOGOUT
ERROR
ERROR

### 出力例

ERROR : 3
LOGIN : 2
LOGOUT : 1

## 要件

- 標準入力からEOFまで読み込む
- イベント名ごとの出現回数を集計する
- アルファベット順に出力する
- 出力形式は

イベント名 : 回数

とする

## 使用してよいクラス

- Scanner
- Map
- TreeMap

## 期待する学習内容

- Mapによる集計
- getOrDefault()
- TreeMapによるキーの自動ソート