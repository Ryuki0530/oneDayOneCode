# SDV Latency Budget Simulator

## 目的

MATLABを用いて、車載システムにおける

- 分散ECU構成
- 統合ECU構成

の処理遅延を簡易的にシミュレーションし、どちらの構成でリアルタイム制約を満たしやすいかを可視化する。

SDVや統合ECUでは、単に処理を1つにまとめれば良いわけではなく、通信遅延、処理遅延、ジッタ、スケジューリング遅延などを考慮する必要がある。

今回はその入口として、MATLAB上で遅延予算をシミュレーションする。

---

## 想定システム

以下のような処理の流れを考える。

```text
Sensor
  ↓
Perception
  ↓
Planning
  ↓
Control
  ↓
Actuator
```

---

## 比較する構成

### 構成A: Distributed ECU

各機能が別ECUに分かれている構成。

```text
Sensor ECU
  ↓ CAN / Ethernet
Perception ECU
  ↓ CAN / Ethernet
Planning ECU
  ↓ CAN / Ethernet
Control ECU
  ↓
Actuator
```

特徴:

- 各処理は比較的軽い
- ECU間通信が多い
- 通信遅延とジッタが発生しやすい

---

### 構成B: Integrated ECU

複数機能を1つの高性能ECUに統合した構成。

```text
Integrated ECU
  - Perception
  - Planning
  - Control
```

特徴:

- ECU間通信は少ない
- ただし、同一ECU内で処理が集中する
- スケジューリング遅延や負荷集中が発生する可能性がある

---

## 実装条件

MATLABスクリプト `sdv_latency_sim.m` を作成する。

試行回数は1000回とする。

```matlab
N = 1000;
```

各処理の遅延は乱数で表現する。

---

## 遅延モデル

### Distributed ECU

以下の平均値と標準偏差を用いる。

| 要素 | 平均[ms] | 標準偏差[ms] |
|---|---:|---:|
| Sensor処理 | 1.0 | 0.2 |
| 通信1 | 2.0 | 0.8 |
| Perception処理 | 8.0 | 1.5 |
| 通信2 | 2.0 | 0.8 |
| Planning処理 | 4.0 | 1.0 |
| 通信3 | 2.0 | 0.8 |
| Control処理 | 2.0 | 0.5 |
| Actuator出力 | 1.0 | 0.2 |

---

### Integrated ECU

以下の平均値と標準偏差を用いる。

| 要素 | 平均[ms] | 標準偏差[ms] |
|---|---:|---:|
| Sensor処理 | 1.0 | 0.2 |
| 内部転送 | 0.5 | 0.2 |
| Perception処理 | 9.0 | 2.0 |
| Planning処理 | 5.0 | 1.5 |
| Control処理 | 3.0 | 1.0 |
| スケジューリング遅延 | 3.0 | 2.0 |
| Actuator出力 | 1.0 | 0.2 |

---

## 制約条件

システム全体の許容遅延を以下とする。

```matlab
deadline_ms = 25;
```

各試行において、合計遅延が25msを超えた場合は deadline miss とする。

---

## 実装すること

1. Distributed ECU構成の合計遅延を1000回分生成する
2. Integrated ECU構成の合計遅延を1000回分生成する
3. それぞれについて以下を計算する
   - 平均遅延
   - 最大遅延
   - 標準偏差
   - deadline miss率
4. ヒストグラムで遅延分布を表示する
5. deadline_ms の位置に縦線を引く
6. 結果を表形式で表示する

---

## 出力例

```text
=== SDV Latency Simulation Result ===

Architecture       Mean[ms]    Max[ms]    Std[ms]    MissRate[%]
Distributed ECU    xx.xx       xx.xx      xx.xx      xx.xx
Integrated ECU     xx.xx       xx.xx      xx.xx      xx.xx
```

---

## 考察すること

実行結果を見て、以下について簡単に考察する。

1. 平均遅延が小さいのはどちらか
2. deadline miss率が小さいのはどちらか
3. Integrated ECUは本当に常に有利と言えるか
4. 分散構成では通信遅延がどのように効いているか
5. 統合構成ではスケジューリング遅延がどのように効いているか

---

## 発展課題

余裕があれば、以下の条件を追加する。

### 発展1

Integrated ECUのスケジューリング遅延を以下の3パターンで比較する。

```text
low load    : mean = 1.0 ms, std = 0.5 ms
middle load : mean = 3.0 ms, std = 2.0 ms
high load   : mean = 7.0 ms, std = 4.0 ms
```

### 発展2

Distributed ECUの通信遅延を以下の2パターンで比較する。

```text
CAN-like      : mean = 2.0 ms, std = 0.8 ms
Ethernet-like : mean = 0.7 ms, std = 0.3 ms
```

---

## 使用するMATLAB要素

```matlab
randn          % 正規分布乱数
max(x, 0)      % 遅延が負にならないようにする
mean           % 平均
std            % 標準偏差
max            % 最大値
sum            % 合計
histogram      % 分布の可視化
xline          % deadlineの縦線
table          % 結果表
disp           % 表示
```

---

## まとめ観点

最終的に、以下のような観点でまとめる。

```text
SDVでは統合ECU化によって通信遅延を削減できる一方、
処理集中によるスケジューリング遅延や負荷変動が新たな問題になる。
そのため、単にECUを統合するだけでなく、
リアルタイム性を保証するOS、ハイパーバイザ、ミドルウェア、通信設計が重要になる。
```