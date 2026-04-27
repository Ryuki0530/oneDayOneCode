# Python課題: Joke Roulette CLI

## 概要

Pythonの `pyjokes` ライブラリを使って、ターミナル上で動く「ジョークルーレット」を作成してください。

実行すると、ランダムなプログラミングジョークを1つ表示します。
さらに、ユーザーがEnterを押すたびに新しいジョークを表示し、`q` を入力すると終了します。

## 使用ライブラリ

- pyjokes

## インストール

```bash
pip install pyjokes
```

## 実行例

```text
=== Joke Roulette ===

Press Enter to get a joke.
Type q to quit.

> 
Why do programmers prefer dark mode? Because light attracts bugs.

> 
There are 10 types of people: those who understand binary and those who don't.

> q
Goodbye!
```

## 要件

1. 起動時にタイトルを表示する
2. Enterキーが押されたら `pyjokes.get_joke()` を使ってジョークを1つ表示する
3. `q` が入力されたら終了する
4. 空白だけの入力もEnterと同じ扱いにする
5. 終了時に `Goodbye!` と表示する

## 余裕があれば追加

- 表示したジョークの数を最後に表示する
- 同じジョークが連続で出ないようにする
- `--count 5` のように、指定した回数だけジョークを表示する機能を追加する

## ファイル名

```text
joke_roulette.py
```

## ヒント

```python
import pyjokes

joke = pyjokes.get_joke()
print(joke)
```