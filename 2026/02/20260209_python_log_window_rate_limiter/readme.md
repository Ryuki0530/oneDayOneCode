# 今日の課題（Python）: Log Window Rate Limiter

## 目的
「直近T秒の間に何回リクエストしたか」を高速に判定する、シンプルなレートリミッタを作る。

## 問題
`RateLimiter` クラスを実装せよ。

- `__init__(limit: int, window_sec: int)`
  - 例: limit=3, window_sec=10 なら「直近10秒に3回まで許可」

- `allow(user_id: str, now: int) -> bool`
  - `now` はUNIX時刻の秒（整数）
  - 許可できるなら `True` を返し、内部状態に今回のアクセスを記録する
  - できないなら `False`（記録しない）
  - 判定前に「now - window_sec より古い記録」は捨てること

## 入出力
標準入力:
- 1行目: `limit window_sec`
- 2行目: `Q`（クエリ数）
- 3行目以降Q行: `now user_id`

標準出力:
- 各クエリごとに `1`(許可) or `0`(拒否)

## 制約（目安）
- Q ≤ 200000
- user_id は英数字
- now は非減少（同時刻はあり）

## 実装条件
- `collections.deque` を使って「各ユーザーのアクセス時刻列」を持つ（推奨）
- 1クエリあたり平均O(1)を狙う

## サンプル
入力:
3 10
7
0 alice
1 alice
2 alice
3 alice
11 alice
12 alice
12 bob

出力:
1
1
1
0
1
1
1