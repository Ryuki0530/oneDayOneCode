# MostFrequentWord

## 問題

文字列のリスト `words` があります。

```python
words = [
    "apple",
    "banana",
    "apple",
    "orange",
    "banana",
    "apple"
]
```

`collections.Counter` を使って各単語の出現回数を数え、
最も多く出現した単語とその回数を表示してください。

## 出力例

```text
apple: 3
```

## 条件

- `collections.Counter` を使用すること
- `for` 文を使って自分でカウント処理を書かないこと
- 最頻出の単語を取得する処理も `Counter` の機能を利用すること

## 目標時間

5〜10分