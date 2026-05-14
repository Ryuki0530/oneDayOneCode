# 今日の1日1コード課題

## タイトル
Log Rotation Simulator

## 問題概要
サーバログを一定件数ごとにローテーションする簡易システムを作成してください。

ログは追加され続けますが、1ファイルに保存できる件数には上限があります。
上限を超えた場合、新しいログファイルへ切り替えます。

## 実装内容

以下のコマンドを順番に処理してください。

- `ADD message`
  - ログを追加する

- `PRINT`
  - 現在保存されている全ログを古い順に出力する

## ログ保存ルール

- 1ファイルに保存できるログ数は `K`
- `K` 件を超えたら新しいログファイルを作る
- ログファイル番号は `1` から始まる
- 出力時は以下形式

[file番号] メッセージ

## 入力形式

K Q
command1
command2
...
commandQ

## 制約

1 <= K <= 100
1 <= Q <= 1000

## 入力例

3 7
ADD boot
ADD login
ADD error
ADD warning
ADD timeout
PRINT
ADD shutdown

## 出力例

[1] boot
[1] login
[1] error
[2] warning
[2] timeout

## ヒント

- ArrayList<ArrayList<String>>
- または List<List<String>>
- 現在のログファイルを管理すると実装しやすいです

## 学べること

- 可変長配列
- ネストしたコレクション
- シミュレーション実装
- 文字列処理