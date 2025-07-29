# 🧩 2025年7月29日 Python課題（軽量アプリケーション）

## 🎯 題名：**Webページの構造を分析・可視化するHTMLパーサーツール**

---

## 🔍 問題概要

`beautifulsoup4` を使い、指定されたWebページのHTML構造を解析し、**各タグの出現回数や階層構造を可視化**するツールを作成せよ。

たとえば、指定URLが `https://example.com` の場合、

- どのタグが何回使われているか（例：`div: 12`, `p: 8`など）
- タグのネストの深さを階層構造として表示（例：`html > body > div > p`）
- 表示形式はCLIでOK（インデントによるツリー構造）

---

## ✅ 要件

### 入力

- ユーザーがURLを指定する（例：`https://example.com`）

### 出力（例）
```
[Tag counts]
html: 1
head: 1
title: 1
body: 1
div: 5
p: 4
a: 3

[Tag hierarchy]
html
└── head
│ └── title
└── body
└── div
├── p
└── a
```
---

## 🔧 使えるライブラリ（インストール済）

- `requests`
- `beautifulsoup4`
- `collections`（標準）
- `re`（標準）

---

## ✍️ 実装ヒント

- `requests.get(url)` でHTML取得
- `BeautifulSoup(html, "html.parser")` でパース
- 再帰関数でDOM階層をたどる
- `collections.Counter` でタグ集計

---

## 🏁 目的

- Webスクレイピング初級〜中級の実用力を養う
- Pythonの再帰・標準データ構造に慣れる
- GUI不要で視覚的に面白さを体験

---

## 💡 拡張アイデア（任意）

- CLI引数でURLを受け取る