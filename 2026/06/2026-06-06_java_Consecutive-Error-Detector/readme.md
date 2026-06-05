# Java課題: Consecutive Error Detector

## 概要

システムログの状態コード列を読み取り、`ERROR` が **K回以上連続** した区間があるか判定してください。

## 入力

```text
N K
S1 S2 ... SN
```

- `N`: ログの件数
- `K`: 警告判定に必要な連続 ERROR 数
- `Si`: 各ログの状態
  - `OK`
  - `WARN`
  - `ERROR`

## 出力

`ERROR` が `K` 回以上連続する箇所があれば

```text
ALERT
```

なければ

```text
SAFE
```

## 制約

- `1 <= N <= 100`
- `1 <= K <= N`

## 入力例1

```text
6 3
OK ERROR ERROR ERROR WARN OK
```

## 出力例1

```text
ALERT
```

## 入力例2

```text
5 2
ERROR OK ERROR WARN ERROR
```

## 出力例2

```text
SAFE
```

## 実装条件

- Javaで実装すること
- ファイル名は `Main.java`
- `Scanner` または `BufferedReader` を使ってよい
- `ERROR` が出たら連続数を増やす
- `ERROR` 以外が出たら連続数を0に戻す
- 連続数が `K` 以上になったら `ALERT`

## 実行例

```bash
javac Main.java
java Main
```