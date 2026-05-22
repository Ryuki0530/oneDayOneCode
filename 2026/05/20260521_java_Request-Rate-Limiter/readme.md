# Task: Request Rate Limiter

## 概要

Web API サーバーでは、同じユーザーから短時間に大量のリクエストが来ると、
サーバー負荷や不正利用の原因になります。

そこで、ユーザーごとに「直近 T 秒以内に K 回まで」しかリクエストを許可しない
簡易レートリミッターを実装してください。

## 問題

N 個のリクエストが時刻順に与えられます。

各リクエストには、時刻 `time` とユーザーID `user` が含まれます。

各リクエストについて、同じユーザーによる直近 `T` 秒以内の許可済みリクエスト数が
`K` 未満であれば、そのリクエストを許可します。

そうでなければ拒否します。

許可されたリクエストは、その後の判定対象に含めます。
拒否されたリクエストは、判定対象に含めません。

## 入力

N K T
time_1 user_1
time_2 user_2
...
time_N user_N

## 制約

- 1 <= N <= 200000
- 1 <= K <= 100
- 1 <= T <= 100000
- 0 <= time_i <= 1000000000
- user_i は英小文字のみからなる文字列
- リクエストは時刻の昇順で与えられる
- 同じ時刻のリクエストが複数存在することがある

## 出力

各リクエストについて、許可する場合は `ALLOW`、
拒否する場合は `DENY` を1行ずつ出力してください。

## 判定ルール

現在のリクエスト時刻を `now` とします。

同じユーザーについて、過去に許可されたリクエストのうち、

now - T < request_time <= now

を満たすものだけを「直近 T 秒以内」として数えます。

つまり、`now - T` 以下の時刻のリクエストは古いものとして削除できます。

## 入力例

8 2 10
0 alice
3 alice
5 bob
10 alice
11 alice
12 bob
13 alice
20 alice

## 出力例

ALLOW
ALLOW
ALLOW
DENY
ALLOW
ALLOW
DENY
ALLOW

## 説明

`alice` は時刻 0, 3 のリクエストが許可されています。

時刻 10 の `alice` のリクエストでは、
直近10秒以内に時刻 0 と 3 の2件が残っているため、
上限 `K=2` に達しており拒否されます。

時刻 11 では、時刻 0 のリクエストは範囲外になるため、
`alice` のリクエストは許可されます。

## 実装方針

ユーザーごとに、許可済みリクエスト時刻を Queue で管理してください。

- `Map<String, ArrayDeque<Integer>>` を使う
- 各リクエストごとに古い時刻を Queue から削除する
- Queue のサイズが K 未満なら許可
- 許可した場合のみ、現在時刻を Queue に追加する

## 使うとよい Java 要素

Map<String, ArrayDeque<Integer>> map = new HashMap<>();

ArrayDeque<Integer> queue =
    map.computeIfAbsent(user, key -> new ArrayDeque<>());

古い時刻の削除例:

while (!queue.isEmpty() && queue.peekFirst() <= now - T) {
    queue.pollFirst();
}

## 今日のポイント

拒否されたリクエストは Queue に入れないことです。

## 目標

- Java の HashMap
- Java の ArrayDeque
- ユーザーごとの状態管理
- 時系列データのスライディングウィンドウ処理

を練習しましょう。