# 今日のJava課題

## 問題名
Access Log Summary

## 問題文
アクセスログが標準入力から複数行与えられます。  
各行は次の形式です。

```text
ユーザー名 操作名
```

例:
```text
alice LOGIN
bob VIEW
alice VIEW
alice LOGOUT
bob VIEW
```

入力の終わりまで読み込み、以下を出力してください。

1. 全ログ件数
2. ユーザーごとの出現回数
3. 操作ごとの出現回数

ただし、出力順は以下のルールに従ってください。

- ユーザーごとの出現回数: ユーザー名の辞書順昇順
- 操作ごとの出現回数: 操作名の辞書順昇順

## 入力例
```text
alice LOGIN
bob VIEW
alice VIEW
alice LOGOUT
bob VIEW
```

## 出力例
```text
TOTAL 5
USERS
alice 3
bob 2
ACTIONS
LOGIN 1
LOGOUT 1
VIEW 3
```

## 条件
- 1行につき空白区切りで2要素
- 入力はEOFまで続く
- 件数は0件以上とする

## ヒント
- `BufferedReader` でEOFまで読む
- 件数集計には `Map<String, Integer>` が使える
- 辞書順で出したいので `TreeMap` が便利

## 目標
- 標準入力を最後まで読む処理に慣れる
- `Map` を使った基本的な集計に慣れる
- Javaでのログ処理の基本を身につける