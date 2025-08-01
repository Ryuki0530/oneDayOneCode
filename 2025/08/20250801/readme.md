# 🐍 Python課題：頻出単語のランキング

## 📘 問題文

あなたは、文章の中で使われている単語の頻度を調べるプログラムを作ることになりました。  
文章を1行入力として受け取り、出現回数が多い順に単語を表示してください。

## 🔽 入力

1行の英小文字のみからなる文章（スペース区切り）  
最大1000文字。句読点は含まれません。

## 🔼 出力

- 各単語とその出現回数を、**出現回数の降順**で出力してください。
- 同じ回数の単語がある場合は**辞書順（昇順）**で並べてください。
- 出力形式： `<単語>: <回数>`

## 🧪 入力例
```
apple banana apple orange banana apple
```
## ✅ 出力例
```
apple: 3
banana: 2
orange: 1
```

## 💡 ヒント

- `collections.Counter` を使うと便利です。
- `sorted()` の `key` 引数でカスタムソートが可能です。

## 💻 動作確認用コード（テンプレート）

```python
from collections import Counter

def main():
    text = input().strip()
    words = text.split()
    counter = Counter(words)

    # 出現回数の降順、単語の昇順でソート
    sorted_items = sorted(counter.items(), key=lambda x: (-x[1], x[0]))

    for word, count in sorted_items:
        print(f"{word}: {count}")

if __name__ == "__main__":
    main()
```

# 実行例（VSCodeターミナルなどで）
```bash
$ python main.py
apple banana apple orange banana apple
apple: 3
banana: 2
orange: 1
```