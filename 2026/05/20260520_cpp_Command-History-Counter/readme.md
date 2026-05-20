# Today’s C++ Practice: Command History Counter

## 問題

コマンドラインツールの操作履歴が `N` 件与えられます。

各履歴は次のいずれかです。

- `OPEN filename`
- `SAVE filename`
- `CLOSE filename`

各 `filename` について、最後に実行された操作を調べてください。

最後の操作が `OPEN` または `SAVE` のファイルは「作業中」とみなし、
そのファイル名を辞書順で出力してください。

作業中のファイルが存在しない場合は `NONE` と出力してください。

## 入力

N
command_1 filename_1
command_2 filename_2
...
command_N filename_N

## 制約

- 1 <= N <= 100000
- command は `OPEN`, `SAVE`, `CLOSE` のいずれか
- filename は英小文字と `.` からなる文字列
- filename の長さは 1 以上 50 以下

## 出力

作業中のファイル名を辞書順で1行ずつ出力してください。

作業中のファイルが存在しない場合は、次を出力してください。

NONE

## 入力例1

6
OPEN main.cpp
SAVE main.cpp
OPEN memo.txt
CLOSE main.cpp
OPEN test.cpp
CLOSE memo.txt

## 出力例1

test.cpp

## 入力例2

4
OPEN a.txt
SAVE a.txt
CLOSE a.txt
CLOSE b.txt

## 出力例2

NONE

## ヒント

- ファイル名ごとに最後の状態を管理する
- 辞書順で出力したいので `map<string, string>` が便利
- 最後に状態が `OPEN` または `SAVE` のものだけ出力する