# Java課題: Safety Mode Simulator

## 問題文

ある組込み機器には、次の4つの動作モードがあります。

- OFF
- IDLE
- RUNNING
- ERROR

初期状態は OFF です。

コマンド列が与えられるので、ルールに従って状態を更新してください。
ルールに存在しない状態遷移が発生した場合、そのコマンドは無効として扱い、状態は変化しません。

最後に、最終状態と無効コマンド数を出力してください。

## 状態遷移ルール

現在の状態 | コマンド | 次の状態
--- | --- | ---
OFF | START | IDLE
IDLE | START | RUNNING
RUNNING | STOP | IDLE
IDLE | STOP | OFF
IDLE | FAIL | ERROR
RUNNING | FAIL | ERROR
ERROR | RESET | IDLE
ERROR | CLEAR | OFF

## 入力

1行目にコマンド数 N が与えられる。

2行目以降に、コマンドが1つずつ与えられる。

入力形式:

N
command_1
command_2
...
command_N

## 出力

最終状態と無効コマンド数を半角スペース区切りで出力せよ。

出力形式:

final_state invalid_count

## 制約

- 1 <= N <= 100
- コマンドは以下のいずれか
  - START
  - STOP
  - FAIL
  - RESET
  - CLEAR

## 入力例

7
START
START
STOP
STOP
RESET
START
FAIL

## 出力例

ERROR 1

## 説明

初期状態は OFF。

1. START: OFF -> IDLE
2. START: IDLE -> RUNNING
3. STOP: RUNNING -> IDLE
4. STOP: IDLE -> OFF
5. RESET: OFF では無効
6. START: OFF -> IDLE
7. FAIL: IDLE -> ERROR

よって、最終状態は ERROR、無効コマンド数は 1。

## 実装条件

- Javaで実装すること
- Scanner または BufferedReader を使って入力すること
- switch文またはMapを使って状態遷移を管理すること