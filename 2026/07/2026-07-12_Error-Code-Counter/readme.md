# OneDayOneCode

## タイトル
Java - Error Code Counter

## 問題

以下のログデータがあります。

```text
INFO
ERROR
WARN
ERROR
INFO
ERROR
WARN
ERROR
ERROR
```

このデータを `List<String>` として保持し、
各ログレベルの出現回数を集計してください。

## 出力例

```text
INFO : 2
WARN : 2
ERROR : 5
```

## 条件

- HashMap を使用すること
- getOrDefault() を使用すること
- for-each を使用すること

## 余裕があれば

出現回数が最も多いログレベルも表示してください。

### 出力例

```text
Most frequent : ERROR (5)
```