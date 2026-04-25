# 座席予約チェッカー

## 概要

映画館の座席予約状況を管理する簡単なプログラムを作成してください。

座席は `N` 個あり、番号は `1` から `N` までです。  
最初はすべて空席です。

予約操作が `Q` 回与えられるので、各操作に対して結果を出力してください。

## 入力形式

```text
N Q
command x
command x
...
```

## コマンド

| command | 内容 |
|---|---|
| reserve | 座席 `x` を予約する |
| cancel | 座席 `x` の予約を取り消す |
| check | 座席 `x` が予約済みか確認する |

## 出力

各操作について以下を出力してください。

### reserve

- 空席なら予約し、`reserved` と出力
- すでに予約済みなら `already reserved` と出力

### cancel

- 予約済みなら取り消し、`canceled` と出力
- 空席なら `not reserved` と出力

### check

- 予約済みなら `reserved` と出力
- 空席なら `available` と出力

## 制約

```text
1 <= N <= 1000
1 <= Q <= 1000
1 <= x <= N
```

## 入力例

```text
5 7
check 3
reserve 3
reserve 3
check 3
cancel 3
cancel 3
check 3
```

## 出力例

```text
available
reserved
already reserved
reserved
canceled
not reserved
available
```

## 実装条件

- Javaで実装すること
- `boolean[]` を使って座席の状態を管理すること
- クラス名は `Main` とすること