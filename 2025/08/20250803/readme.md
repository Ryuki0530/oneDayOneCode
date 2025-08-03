# C言語問題：ファイル内の単語頻度カウント

## 問題概要

標準入力からファイル名を1つ受け取り、そのファイル内のすべての英単語の出現回数をカウントして表示するプログラムを作成してください。

## 仕様

- 対象とする英単語は、アルファベットの連続（大文字小文字は区別しない）です。
- 単語の区切りはスペースや改行、句読点などの非アルファベット文字とします。
- 単語はすべて小文字に変換して処理してください。
- 単語ごとに出現回数を表示し、出現回数の多い順に並べてください（回数が同じ場合は辞書順）。

## 入力

- 標準入力からファイル名（最大255文字）を1つ受け取る。

例：
sample.txt

- `sample.txt` の中身（例）：
Hello world! Hello again.
This is a test. A simple, simple test.


## 出力（例）
```
a: 2
hello: 2
simple: 2
test: 2
again: 1
is: 1
this: 1
world: 1
```

## 制約

- ファイルサイズは最大で10KBとする。
- 出現単語数は最大500語とする。

## 実行例（Linux環境）

```bash
$ gcc word_count.c -o word_count
$ ./word_count
sample.txt
ヒント
fopen, fgets, isalpha, tolower, strtok, strcmp, qsort などを活用しましょう。

単語の重複チェックには構造体と配列を用いるのが簡単です。

```