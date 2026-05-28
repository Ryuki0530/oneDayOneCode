# Java課題: Status Log Counter

## 目的
ログ文字列を読み取り、状態ごとの件数を集計する。

## 問題文
N個のログが与えられる。
各ログは次の形式で与えられる。

```text
時刻 状態
```

状態は以下の3種類のいずれかである。

- OK
- WARN
- ERROR

それぞれの状態が何回出現したかを数えて出力せよ。

## 入力

```text
N
time_1 status_1
time_2 status_2
...
time_N status_N
```

## 出力

```text
OKの件数 WARNの件数 ERRORの件数
```

## 制約

- 1 <= N <= 1000
- time は文字列
- status は OK, WARN, ERROR のいずれか

## 入力例

```text
6
10:00 OK
10:01 WARN
10:02 OK
10:03 ERROR
10:04 WARN
10:05 OK
```

## 出力例

```text
3 2 1
```

## 実装条件

- Javaで実装すること
- クラス名は `Main`
- `Scanner` または `BufferedReader` を使ってよい

## ヒント

`HashMap<String, Integer>` を使うと実務寄りです。

ただし、今回は時間がなければ次の3変数で十分です。

```java
int ok = 0;
int warn = 0;
int error = 0;
```

文字列比較は `==` ではなく、次のように行います。

```java
status.equals("OK")
```