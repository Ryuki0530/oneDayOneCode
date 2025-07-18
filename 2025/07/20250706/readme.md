# 📅 2025-07-06 Java課題：社員データの管理

## 📝 問題文

ある会社では、社員のデータを以下のような形式でファイルに記録しています。

```
社員ID,名前,部署,年齢
E001,田中太郎,営業,28
E002,佐藤花子,開発,35
E003,鈴木一郎,営業,41
```
...


このCSVファイル（`employees.csv`）を読み込み、以下の処理を行うJavaプログラムを作成してください。

---

## 🎯 要件

### 1. ファイルの読み込み
- `employees.csv` というファイルを読み込み、各行を社員オブジェクトとして格納してください。

### 2. 検索機能の実装
- ユーザーから**部署名**を入力として受け取り、その部署に所属している社員の一覧を出力してください。

### 3. 平均年齢の表示
- 指定された部署の社員の**平均年齢**を計算して表示してください（少数第1位まで）。

---

## 💡 入出力例

### 📂 `employees.csv`
```
E001,田中太郎,営業,28
E002,佐藤花子,開発,35
E003,鈴木一郎,営業,41
E004,山田健一,人事,30
E005,田村幸子,開発,29
```

### 💬 入力（標準入力）
検索したい部署を入力してください:
```
開発
```

### 💻 出力（標準出力）
```
部署「開発」の社員一覧:

E002 佐藤花子 (35歳)

E005 田村幸子 (29歳)

平均年齢: 32.0歳
```
---

## 📌 制約
- ファイル名は `"employees.csv"` と固定してください。
- 社員数は最大で100人と仮定してよい。
- 年齢は整数とし、平均は `double` で計算。

---

## 🧩 発展課題（任意）

1. 年齢の高い順にソートして表示してみましょう。
2. 該当部署が存在しない場合、エラーメッセージを表示してください。

---

## 🛠 ヒント

- ファイルの読み込みには `BufferedReader` や `Files.readAllLines()` を使うと便利です。
- 社員情報を格納するために `Employee` クラスを定義するとよいでしょう。