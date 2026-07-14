# OneDayOneCode

## タイトル
Log Level Counter

## 問題
以下のログをList<String>として保持し、ログレベルごとの件数を集計してください。

```text
INFO: Application started
WARN: Low memory
INFO: User login
ERROR: Database connection failed
INFO: File loaded
ERROR: Timeout
WARN: Disk usage high
INFO: Shutdown
```

## 要件

- `Map<String, Integer>` を使用して集計すること
- 出力形式は以下の通り

```text
INFO : 4
WARN : 2
ERROR: 2
```

## 発展（時間があれば）

ログレベルをアルファベット順ではなく、

- INFO
- WARN
- ERROR

の順番で表示してください。