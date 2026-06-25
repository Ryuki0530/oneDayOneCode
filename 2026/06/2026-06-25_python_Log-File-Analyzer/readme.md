# OneDayOneCode

## タイトル
Log File Analyzer

## 難易度
★★☆☆☆

## 所要時間
20～30分

## 問題

ログファイルを解析する簡単なプログラムを作成してください。

入力ファイル(example.log)

INFO System started
INFO User login
WARNING Disk usage high
ERROR Database connection failed
INFO Retry database
ERROR Database connection failed
WARNING Memory usage high
INFO Shutdown

プログラムはファイルを読み込み、以下を表示してください。

=== Log Summary ===
INFO    : 4
WARNING : 2
ERROR   : 2

Most Frequent Level : INFO

## 条件

- collections.Counter を利用してよい
- ファイル名はコマンドライン引数から受け取る
- 存在しないファイルの場合は
  "File not found"
  と表示して終了する
- 関数に分割すること

## 期待する関数

read_log(path)
count_levels(lines)
print_summary(counter)

## 発展課題（時間があれば）

ERROR行だけを最後に一覧表示する。

例

=== Error Messages ===
Database connection failed
Database connection failed