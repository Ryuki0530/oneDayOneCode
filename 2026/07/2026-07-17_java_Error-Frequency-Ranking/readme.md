# OneDayOneCode

## タイトル
Java - Error Frequency Ranking

## 問題

次のログレベル一覧が与えられます。

```java
List<String> logs = List.of(
    "INFO",
    "ERROR",
    "WARN",
    "ERROR",
    "INFO",
    "ERROR",
    "WARN",
    "DEBUG",
    "INFO",
    "ERROR"
);
```

各ログレベルの出現回数を集計し、出現回数の多い順に表示してください。

### 出力例

ERROR : 4
INFO : 3
WARN : 2
DEBUG : 1

## 条件

- HashMap を利用すること
- Stream APIを使っても使わなくてもよい
- 出力は件数の降順で並べること

## 学べること

- HashMapによる頻度集計
- Map.Entry の扱い
- Comparatorを使ったソート
- Javaでよくある「集計→ランキング表示」の流れ