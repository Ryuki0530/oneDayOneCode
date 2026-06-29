# OneDayOneCode

## タイトル
Log Level Counter

## 問題
ログファイルを表す文字列の配列が与えられる。

```java
String[] logs = {
    "INFO:Start",
    "WARN:Low memory",
    "INFO:Login",
    "ERROR:File not found",
    "INFO:Logout",
    "WARN:High CPU"
};
```

各ログレベル(INFO, WARN, ERROR)が何件あるか集計し、以下のように表示してください。

### 出力例

INFO : 3
WARN : 2
ERROR : 1

## 条件
- HashMap<String, Integer> を使用すること。
- ":" の前をログレベルとして取得すること。
- 存在しないキーは0件として扱うこと。
- ログレベルが増えても動作する実装にすること（if文を大量に書かない）。

## 学習ポイント
- HashMap
- getOrDefault()
- split()
- 拡張しやすい集計処理