# 今日の課題: ログ集計ミニシステム

## テーマ
イベントログを読み取り、ユーザーごとの行動回数を集計するプログラムを作成してください。

---

## 問題文

あなたは簡易ログ解析ツールを作成します。  
標準入力からイベントログが複数行与えられるので、以下の集計結果を出力してください。

各ログは次の形式です。

<timestamp> <userId> <action>

例:
1 alice LOGIN

`timestamp` は整数、`userId` と `action` は英数字の文字列です。

入力の終わりまで読み込んだら、以下を出力してください。

1. 全ログ数
2. ユーザーごとのログ数（ユーザーIDの辞書順）
3. アクションごとのログ数（アクション名の辞書順）

---

## 入力例

1 alice LOGIN
2 bob VIEW
3 alice VIEW
4 alice LOGOUT
5 bob VIEW
6 bob LOGOUT

---

## 出力例

TOTAL 6
USERS
alice 3
bob 3
ACTIONS
LOGIN 1
LOGOUT 2
VIEW 3

---

## 条件

- 入力は EOF まで続く
- 1行は必ず `timestamp userId action` の3要素
- `timestamp` は集計に使わなくてもよい
- ユーザーごとの件数、アクションごとの件数はそれぞれ辞書順で出力すること

---

## 期待する学習ポイント

- `BufferedReader` を使った入力
- `String.split()` による文字列分割
- `Map` を使った件数集計
- `TreeMap` を使った辞書順管理
- 標準入力が EOF になるまで読む方法

---

## ヒント

- 全体件数は `int total` で管理できます
- ユーザー集計は `Map<String, Integer>`
- アクション集計も `Map<String, Integer>`
- 辞書順で出したいので `TreeMap` を使うと楽です
- 件数更新は `map.put(key, map.getOrDefault(key, 0) + 1);` が便利です

---

## 追加課題（余裕があれば）

### 追加課題1
最も多いアクションを1つ出力してください。  
件数が同じ場合は辞書順で小さいものを優先してください。

出力例:
TOP_ACTION VIEW 3

### 追加課題2
特定ユーザーだけを集計できるようにしてください。  
最初の1行に対象ユーザーIDが与えられ、`ALL` のときは全件対象にする仕様にしてみましょう。

---

## 関連モジュール・文法メモ

### 1. BufferedReader
高速に文字列入力を受け取る定番です。

```java
BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
```
### 2. EOF まで読む
```java
String line;
while ((line = br.readLine()) != null) {
    // 処理
}
```
### 3. 文字列分割
```java
String[] parts = line.split(" ");
```
### 4. Map でカウント
```java
map.put(key, map.getOrDefault(key, 0) + 1);
```
### 5. TreeMap

キーを自動で辞書順に並べたいときに便利です。
```java
Map<String, Integer> userCount = new TreeMap<>();
```