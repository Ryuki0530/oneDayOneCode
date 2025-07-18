# 📘 20250630：Java課題 - ファイル拡張子別集計プログラム

## 📝 問題内容

指定されたディレクトリ内に存在するすべての**ファイル**（サブディレクトリは無視）を調べて、ファイルの拡張子ごとの個数をカウントしてください。結果は、拡張子順にソートして表示します。

---

## 📥 入力
- 標準入力から **対象のディレクトリパス** を1行で受け取る。

---

## 📤 出力
- 各拡張子と、その個数を以下の形式で標準出力に表示してください（拡張子順）：

拡張子: 個数

例：
```
csv: 2
java: 5
txt: 10
```

---

## ✅ 条件
- 拡張子は `"."` の後の文字列です（例：`Main.java` の拡張子は `java`）。
- 拡張子が存在しないファイルはカウント対象外とします。
- 大文字・小文字は区別せず、小文字に統一してカウントしてください。
- `Files.walk` や `File` クラスなどを用いても構いません。

---

## 💡 実行例

### ディレクトリ構成：
```
test_dir/
├── Main.java
├── Utils.java
├── README.txt
├── config.csv
├── data.csv
├── LICENSE
```

### 入力：
`test_dir`


### 出力：
```
csv: 2
java: 2
txt: 1
```
（※ `LICENSE` は拡張子なしなので対象外）

---

## 💡 ヒント
- `File` クラスでファイル一覧を取得し、拡張子を取り出すために `lastIndexOf('.')` を使うと便利です。
- ソートには `TreeMap` を使うと簡単です。

---
