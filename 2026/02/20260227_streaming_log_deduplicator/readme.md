# 今日のPython課題：Streaming Log Deduplicator（重複排除つきログ集計）

## 概要
標準入力で流れてくる「イベントログ」を処理して、**重複（一定時間内に同一イベント）を無視**しつつ、集計結果を出すミニツールを作ってください。  
リアル運用っぽい「ログのノイズ除去＋集計」を狙った問題です。

---

## 入力形式
1行に1イベント。形式は以下：

`<timestamp_ms> <user_id> <event_name>`

- `timestamp_ms`：ミリ秒の整数（単調増加とは限らない）
- `user_id`：英数字文字列
- `event_name`：英数字と`_`のみ（例：`page_view`, `click_buy`）

---

## 重複判定ルール（重要）
同一の `(user_id, event_name)` について、直近で採用されたイベントの時刻を `t_last` とすると、

- 新しいイベント時刻が `t` のとき  
  **`t - t_last <= W` なら重複として捨てる**  
  **`t - t_last > W` なら採用**

ここで `W` はウィンドウ幅（ミリ秒）で、最初の入力行で与えられます。

---

## 入力の先頭
最初の1行はウィンドウ幅：

`W`

（例：`3000`）

---

## 出力
採用されたイベントだけを集計して、以下を出力してください。

1. 採用イベント総数 `TOTAL <n>`
2. event_name ごとの採用数を多い順（同数なら辞書順）で列挙  
   `EVENT <event_name> <count>`
3. user_id ごとの採用数を多い順（同数なら辞書順）で列挙  
   `USER <user_id> <count>`

---

## 例

### 入力
```
3000
1000 u1 page_view
1500 u1 page_view
5000 u1 page_view
2000 u2 click_buy
4000 u2 click_buy
7000 u2 click_buy
2500 u1 click_buy
2600 u1 click_buy
```


### 説明（W=3000）
- u1/page_view: 1000 採用、1500 は 500ms差で捨て、5000 は 4000ms差で採用
- u2/click_buy: 2000 採用、4000 は 2000ms差で捨て、7000 は 5000ms差で採用
- u1/click_buy: 2500 採用、2600 は 100ms差で捨て

### 出力例
```
TOTAL 5
EVENT click_buy 3
EVENT page_view 2
USER u1 3
USER u2 2
```


---

## 制約（目安）
- 行数 N は最大 2 * 10^5 程度を想定
- なるべく **O(N)** で処理（ソートは出力整形でOK）

---

## 実装条件
- Python標準ライブラリのみ
- 入力は `sys.stdin.buffer` 推奨（高速化のため）

---

## ヒント（使うと良いモジュール）
- `sys`：高速入力
- `collections`：`defaultdict(int)` で集計
- 辞書：`last_accepted[(user_id, event_name)] = timestamp`

---

## 追加チャレンジ（余裕があれば）
- timestamp が「単調増加ではない」場合にも正しく動くようにする（この課題のルール通りだと、そのままでも動くが、設計を丁寧に）
- `t - t_last` が負になるケース（過去時刻が来る）をどう扱うかを自分で決め、READMEに書く  
  例：負なら「重複扱いせず採用」or「無視」など。自分の方針を明記。

---