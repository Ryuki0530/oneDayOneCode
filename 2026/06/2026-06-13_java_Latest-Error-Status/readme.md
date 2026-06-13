# Java課題: Latest Error Status

## 概要
複数の機器から送られてくる状態ログを処理し、最後に各機器が異常状態かどうかを判定してください。

## 入力
1行目にログ数 N が与えられる。

続く N 行に以下の形式でログが与えられる。

device_id status

- device_id は機器IDを表す文字列
- status は OK または ERROR

## 出力
最後の状態が ERROR である機器IDを、辞書順に1行ずつ出力してください。

該当する機器がない場合は、以下を出力してください。

ALL_OK

## 制約
- 1 <= N <= 100000
- device_id は英数字のみ
- Javaで実装すること

## 入力例1
6
motor1 OK
sensor2 ERROR
motor1 ERROR
camera1 OK
sensor2 OK
battery ERROR

## 出力例1
battery
motor1

## 入力例2
3
a OK
b ERROR
b OK

## 出力例2
ALL_OK

## ヒント
- HashMap<String, String> で機器ごとの最新状態を管理する
- 最後に ERROR の機器だけ ArrayList に集める
- 辞書順ソートには Collections.sort() を使う