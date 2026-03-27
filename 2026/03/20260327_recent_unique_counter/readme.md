```markdown
# 今日のPython課題: Recent Unique Counter

## 問題
イベント列が標準入力から与えられます。
各行は次の形式です。

`user_id item_id`

同じ `user_id` が同じ `item_id` を何回送っても、そのユーザーにとっては1回だけ出現したものとして数えます。

入力全体を読み終えたら、各 `user_id` について「異なる `item_id` を何種類送ったか」を出力してください。

ただし、出力は `user_id` の辞書順で行ってください。

---

## 入力例
```text
alice apple
bob book
alice orange
alice apple
bob pen
charlie note
bob book
```

## 出力例
```text
alice 2
bob 2
charlie 1
```

---

## 条件
- 1行に1イベント
- 入力の終わりまで読み込む
- 同じ `(user_id, item_id)` の重複は1回として扱う
- 出力は `user_id` の辞書順

---

## ヒント
- `dict`
- `set`
- `sys.stdin`

---

## 余裕があれば
各ユーザーについて件数だけでなく、重複を除いた `item_id` 一覧も辞書順で出力してみてください。
```