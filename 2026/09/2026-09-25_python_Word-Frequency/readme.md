# WordFrequency

## 問題

文章に含まれる単語の出現回数を集計し、
出現回数が多い順に表示するプログラムを作成してください。

Python標準ライブラリの `collections.Counter` を使用してください。

## 条件

1. 以下の文章を使用する。

text = """
Python is easy to learn.
Python is powerful and Python is useful.
Learning Python is fun.
"""

2. 大文字・小文字は区別しない。
   - "Python" → "python"

3. `.` や `,` などの記号は単語として扱わない。

4. `collections.Counter` を使用して単語数を集計する。

5. 出現回数が多い順に表示する。

## 出力例

python: 4
is: 4
easy: 1
to: 1
learn: 1
powerful: 1
and: 1
useful: 1
learning: 1
fun: 1

※ 同じ出現回数の単語について、表示順は問いません。

## 発展課題

最も多く出現した単語を1つだけ、

Most common word: python (4)

の形式で表示してください。

## 使用ファイル

main.py