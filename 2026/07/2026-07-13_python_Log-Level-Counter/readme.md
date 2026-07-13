# OneDayOneCode

## タイトル
Python Log Level Counter

## 問題

ログメッセージの一覧から、ログレベルごとの件数を集計してください。

入力データ

logs = [
    "INFO: Start",
    "ERROR: File not found",
    "INFO: Load config",
    "WARNING: Low memory",
    "ERROR: Connection failed",
    "INFO: Ready"
]

期待する出力

INFO: 3
WARNING: 1
ERROR: 2

## 条件

- 辞書(dict)を使って集計すること
- ログレベルは ":" より前の文字列とする
- 存在しないキーは適切に初期化すること
- collections.Counterは使用しない

## 発展課題（時間があれば）

ログレベル順ではなく、件数が多い順に表示してください。

期待例

INFO: 3
ERROR: 2
WARNING: 1