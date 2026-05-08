# 今日の課題: Stack Plate Counter

## 概要

皿を積み重ねる簡易シミュレータを作成してください。

皿には色を表す文字列が書かれています。
操作は以下の3種類です。

- `PUSH color`
  - 色 `color` の皿を一番上に積む
- `POP`
  - 一番上の皿を取り除く
  - 皿が1枚もない場合は何もしない
- `TOP`
  - 一番上の皿の色を出力する
  - 皿が1枚もない場合は `EMPTY` を出力する

## 入力

```text
N
command_1
command_2
...
command_N
```

## 制約

- `1 <= N <= 100`
- `color` は英小文字のみ
- `color` の長さは 1 以上 20 以下

## 出力

`TOP` 操作ごとに結果を1行で出力してください。

## 入力例

```text
7
TOP
PUSH red
PUSH blue
TOP
POP
TOP
POP
```

## 出力例

```text
EMPTY
blue
red
```

## 実装条件

- Javaで実装すること
- クラス名は `Main`
- `ArrayDeque<String>` を使うこと

## ヒント

Javaではスタック用途に `ArrayDeque` が使えます。

```java
ArrayDeque<String> stack = new ArrayDeque<>();

stack.push("red");  // 追加
stack.pop();        // 取り出し
stack.peek();       // 一番上を見る
stack.isEmpty();    // 空か確認
```

## ブランチ名案

```text
java-stack-plate-counter
```