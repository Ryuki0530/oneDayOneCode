# OneDayOneCode
## 2026-07-05
# Python
# Project Name
Human Terminal Recorder

## 概要

「人間のターミナル操作を記録し、あとから再生できるツール」を作成してください。

LinuxやWSLで作業していると、

「あれ、このコマンド何だったっけ？」
「前回どうやって環境構築したっけ？」

となることが多くあります。

そこで、自分の入力したコマンドとその結果を記録し、
あとから閲覧できるCLIツールを作ります。

---

## 要件

起動すると

> record

モードになる。

入力例

$ ls
<結果>

$ pwd
<結果>

$ git status
<結果>

...

ユーザーが

exit

と入力するまで繰り返す。

---

## 保存形式

history.json

例

[
  {
    "time":"2026-07-05T18:30:21",
    "command":"pwd",
    "returncode":0,
    "stdout":"/home/user/project",
    "stderr":""
  },
  ...
]

---

## 実装条件

Python標準ライブラリのみ

利用してよいもの

- subprocess
- pathlib
- json
- datetime
- shlex

---

## Bonus1

history show

を実装し、

過去の履歴を一覧表示する。

---

## Bonus2

history grep git

のようにすると

git

を含むコマンドだけ表示する。

---

## Bonus3

history replay 15

で15番目のコマンドを再実行できるようにする。

※実行前に確認メッセージを表示すること。

---

## Bonus4（おすすめ）

実行時間も保存する。

開始時刻

終了時刻

実行時間(ms)

をJSONへ保存する。

---

## Bonus5（上級）

よく使うコマンドランキングを表示する。

例

Top Commands

git      35
ls       24
python   18
cd       15

---

## 学べること

- subprocess.run()
- コマンドラインツール設計
- JSONログ設計
- シェルの仕組み
- Pythonでの開発支援ツール作成09j