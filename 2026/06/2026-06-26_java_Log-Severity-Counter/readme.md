# OneDayOneCode

## Title
Log Severity Counter

## 問題

ログファイルを模した文字列配列が与えられます。

```java
String[] logs = {
    "[INFO] Start",
    "[WARN] Low Memory",
    "[INFO] Connect",
    "[ERROR] File Not Found",
    "[WARN] Retry",
    "[ERROR] Timeout",
    "[INFO] End"
};
```

各ログレベル(INFO / WARN / ERROR)の件数を集計してください。

## 出力例

INFO : 3
WARN : 2
ERROR: 2

## 条件

- Map<String, Integer> を利用すること
- if文やswitch文でログレベルを判定してよい
- count = map.getOrDefault(...) を活用すること

## 発展課題（余裕があれば）

ログを日時付きに変更します。

```java
"[2026-06-26 20:10:05][ERROR] Timeout"
```

この場合でも ERROR / WARN / INFO を正しく取り出して集計してください。