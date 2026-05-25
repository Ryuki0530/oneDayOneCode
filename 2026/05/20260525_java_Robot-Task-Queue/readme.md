# Java課題: Robot Task Queue

## 目的

ロボットや自動化システムで使われる「タスクキュー」を簡単にシミュレーションするプログラムを作成してください。

## 問題文

ロボットに対して、複数のコマンドが順番に与えられます。

ロボットはタスクを1つずつキューに追加し、先頭から実行します。

各タスクには以下の情報があります。

- タスクID
- タスク名
- 実行に必要な時間

コマンドは次の3種類です。

- ADD id name time
  - タスクをキューの末尾に追加する
- RUN
  - キューの先頭のタスクを1つ実行する
- STATUS
  - 現在キューに残っているタスク数と、合計必要時間を出力する

ただし、同じタスクIDのタスクはキュー内に同時に存在できません。

すでに同じIDのタスクがある状態で ADD が来た場合、そのタスクは追加せず、以下を出力してください。

DUPLICATE id

RUN のとき、キューが空なら以下を出力してください。

EMPTY

RUN に成功した場合は、以下を出力してください。

RUN id name time

STATUS の場合は、以下を出力してください。

STATUS count total_time

## 入力形式

N
command_1
command_2
...
command_N

## 制約

- 1 <= N <= 1000
- id は整数
- 1 <= id <= 100000
- name は英小文字のみ
- 1 <= time <= 1000
- command は ADD, RUN, STATUS のいずれか

## 入力例

8
ADD 1 move 10
ADD 2 grab 5
STATUS
ADD 1 rotate 7
RUN
STATUS
RUN
RUN

## 出力例

STATUS 2 15
DUPLICATE 1
RUN 1 move 10
STATUS 1 5
RUN 2 grab 5
EMPTY

## 補足

最初に move と grab が追加されます。

STATUS の時点では、タスク数は2、合計時間は15です。

その後、ID 1 のタスクを再度 ADD しようとしていますが、
ID 1 はまだキュー内に存在するため追加できません。

RUN で move が実行されると、ID 1 はキューから消えるため、
以降は同じIDのタスクを再度追加できるようになります。

## 実装条件

- Javaで実装してください
- クラス名は Main としてください
- Scannerではなく BufferedReader の使用を推奨します
- Queue または ArrayDeque を使ってください
- 重複IDの管理には HashSet を使ってください
- 合計必要時間 total_time は毎回計算し直さず、変数で管理してください