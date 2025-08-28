# 課題: JSONデータの集計と整形出力

## 問題文
あなたは書籍情報を扱うプログラムを作成することになりました。  
入力として、以下の形式のJSONファイル `books.json` が与えられます。

```json
[
  {"title": "Python入門", "author": "Tanaka", "price": 2800, "year": 2021},
  {"title": "アルゴリズム図鑑", "author": "Sato", "price": 3200, "year": 2019},
  {"title": "機械学習ハンドブック", "author": "Suzuki", "price": 4500, "year": 2023},
  {"title": "データベース基礎", "author": "Tanaka", "price": 3000, "year": 2018}
]
```
このデータを読み込み、以下の処理を行うプログラムを作成してください。

全ての本の冊数と合計金額を出力

## 出力例:
```bash
Total Books: 4
Total Price: 13500
```

著者ごとの冊数と平均価格を出力

## 出力例:
```bash
Author: Tanaka, Books: 2, Avg Price: 2900.0
Author: Sato, Books: 1, Avg Price: 3200.0
Author: Suzuki, Books: 1, Avg Price: 4500.0
```
出版年が新しい順にソートしてタイトル一覧を出力

## 出力例:
```bash
Books by Year (Newest First):
2023: 機械学習ハンドブック
2021: Python入門
2019: アルゴリズム図鑑
2018: データベース基礎
```
## 入出力形式
入力: books.json（上記形式のJSONファイル）
出力: 上記指定の形式を標準出力へ出力する。

## 制約
標準ライブラリのみ使用可能。

JSONのキーは必ず存在するものとする。

---
