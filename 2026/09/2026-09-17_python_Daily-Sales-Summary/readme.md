# DailySalesSummary

## 問題

商品の売上データが、次のようなタプルのリストとして与えられます。

```python
sales = [
    ("apple", 120),
    ("banana", 80),
    ("apple", 150),
    ("orange", 100),
    ("banana", 70),
    ("apple", 130),
]
```

各タプルは、

```text
(商品名, 売上金額)
```

を表しています。

このデータを商品ごとに集計し、

- 合計売上
- 販売回数
- 平均売上

を表示するプログラムを作成してください。

## 条件

- `collections.defaultdict` を使用すること
- 商品名が何種類あるかは事前には分からないものとする
- 平均売上は小数で表示してよい
- 集計処理は `summarize_sales()` 関数として実装すること

## 関数

```python
def summarize_sales(sales):
    pass
```

## 出力例

```text
apple: total=400, count=3, average=133.33
banana: total=150, count=2, average=75.00
orange: total=100, count=1, average=100.00
```

※ 出力順序は問いません。

## 発展

余裕があれば、合計売上が最も大きい商品を最後に

```text
Best seller: apple
```

のように表示してください。