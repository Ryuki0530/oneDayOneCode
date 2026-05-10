# 今日の課題：Priority Help Desk

## 概要

ヘルプデスクに問い合わせが届きます。

問い合わせには、名前 `name` と優先度 `priority` が設定されています。
優先度の値が小さい問い合わせほど先に対応されます。

同じ優先度の場合は、先に追加された問い合わせを先に対応します。

## 入力

```text
Q
query_1
query_2
...
query_Q
```

各クエリは次のどちらかです。

```text
ADD name priority
CALL
```

- `ADD name priority`
  - 問い合わせを追加する
- `CALL`
  - 次に対応する問い合わせの名前を出力する
  - 問い合わせが1件もない場合は `NONE` を出力する

## 制約

- `1 <= Q <= 100000`
- `name` は英小文字のみからなる文字列
- `1 <= priority <= 100000`

## 出力

`CALL` クエリごとに結果を1行で出力してください。

## 入力例

```text
7
ADD alice 3
ADD bob 1
ADD carol 1
CALL
ADD dave 2
CALL
CALL
```

## 出力例

```text
bob
carol
dave
```

## ヒント

Pythonでは `heapq` を使うと優先度付きキューを実装できます。

同じ優先度の順番を保つために、追加順を表す番号 `order` も一緒に入れるとよいです。

例：

```python
heapq.heappush(heap, (priority, order, name))
```