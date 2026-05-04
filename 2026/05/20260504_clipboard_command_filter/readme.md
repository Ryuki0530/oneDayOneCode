# Daily Problem: Clipboard Command Filter

## 概要

複数行のコマンド履歴から、実行対象にするコマンドだけを抽出してください。

## 問題文

N 個のコマンド履歴が与えられます。

各行には、次のいずれかの形式の文字列が与えられます。

- `RUN command`
- `SKIP command`

`RUN` で始まる行だけを実行対象とし、その後ろの `command` 部分を順番に出力してください。

## 入力形式

```text
N
line_1
line_2
...
line_N
```

## 出力形式

`RUN` の行に含まれるコマンドだけを、入力順に1行ずつ出力してください。

## 制約

- 1 <= N <= 100
- command は空白を含む場合がある
- 各行の長さは 100 文字以下

## 入力例

```text
5
RUN python main.py
SKIP rm temp.txt
RUN git status
RUN echo hello world
SKIP shutdown
```

## 出力例

```text
python main.py
git status
echo hello world
```

## 実装条件

- Pythonで実装すること
- `split(" ", 1)` を使って、先頭の命令とコマンド部分を分けること

## ヒント

```python
kind, command = line.split(" ", 1)
```

`split()` だけだと `echo hello world` のような空白を含むコマンドが壊れるので注意してください。