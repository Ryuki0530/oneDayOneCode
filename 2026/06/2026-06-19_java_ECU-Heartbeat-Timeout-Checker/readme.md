# ECU-Heartbeat-Timeout-Checker

## 目的

車載ECUの死活監視をイメージして、一定周期で届くはずのハートビートログを解析し、通信途絶を検出するJavaプログラムを作成する。

## 想定時間

20〜30分

## 入力

標準入力から以下の形式でログを受け取る。

```text
N TIMEOUT_MS
timestamp_ms ecu_name
timestamp_ms ecu_name
...
```

- `N` はログ行数
- `TIMEOUT_MS` は許容する最大間隔
- `timestamp_ms` はミリ秒単位の時刻
- `ecu_name` はECU名
- ログは時刻昇順で与えられる

## 出力

同じECUから次のハートビートが届くまでの間隔が `TIMEOUT_MS` を超えた場合、以下の形式で出力する。

```text
TIMEOUT ecu_name previous_timestamp current_timestamp interval_ms
```

タイムアウトが1件もない場合は、以下を出力する。

```text
OK
```

## 入力例

```text
8 100
0 EngineECU
20 BrakeECU
80 EngineECU
130 BrakeECU
250 EngineECU
260 BrakeECU
420 EngineECU
430 BrakeECU
```

## 出力例

```text
TIMEOUT EngineECU 80 250 170
TIMEOUT BrakeECU 130 260 130
TIMEOUT EngineECU 250 420 170
TIMEOUT BrakeECU 260 430 170
```

## 実装条件

- 言語はJava
- `Main.java` として作成する
- 標準入力から読み込む
- `HashMap<String, Long>` を使って、ECUごとの直前受信時刻を管理する
- タイムアウト検出結果は、発見した順に出力する

## 考え方

ECU名をキー、直前に受信した時刻を値として `HashMap` に保存する。

同じECUの次のログが来たら、

```text
現在時刻 - 前回時刻
```

を計算する。

この値が `TIMEOUT_MS` より大きければ、通信途絶として `TIMEOUT` を出力する。

## 追加課題

余裕があれば、ECUごとの受信回数も数える。

出力例：

```text
COUNT EngineECU 4
COUNT BrakeECU 4
```

ただし、時間がない場合は必須ではない。