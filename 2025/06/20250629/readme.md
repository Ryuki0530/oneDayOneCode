# 📘 20250629：Python課題 - ログファイルからのエラーメッセージ抽出

## 🔍 問題概要

`log.txt` という名前のログファイルには、システムやアプリケーションの動作中に出力された様々なメッセージが記録されています。

この中から、以下の条件を満たす**エラーメッセージ**を抽出し、新しいファイル `error_summary.txt` に出力するプログラムを作成してください。

---

## 📁 入力ファイル: `log.txt`

各行には次の形式でログが記録されています：

[YYYY-MM-DD HH:MM:SS] LEVEL: メッセージ<br>


例えば：

```
[2025-06-29 12:01:02] INFO: Application started
[2025-06-29 12:01:05] ERROR: Failed to connect to database
[2025-06-29 12:01:06] WARNING: Disk space low
[2025-06-29 12:01:07] ERROR: Timeout while reading configuration
```


---

## ✅ 要件

1. **"ERROR"レベルのログ行のみ**を抽出する。
2. 抽出した行の**日時とメッセージのみ**を取り出して、次の形式で `error_summary.txt` に書き込む：
```
2025-06-29 12:01:05 - Failed to connect to database
2025-06-29 12:01:07 - Timeout while reading configuration
```
3. `error_summary.txt` は上書きで構わない。

---

## 💡 ヒント

- 正規表現を活用すると便利です。
- `with open(...)` を使ってファイルの読み書きを行ってください。
- 抽出には `re` モジュールを使いましょう。

---

## 🧪 サンプル実行後の `error_summary.txt`
```
2025-06-29 12:01:05 - Failed to connect to database
2025-06-29 12:01:07 - Timeout while reading configuration
```

---

## 🧩 応用課題（余裕があれば）

- `ERROR` の出現回数をカウントして、プログラムの最後に `print()` で表示する。
- 同時に `WARNING` ログも集計し、別ファイルに出力する。

---
