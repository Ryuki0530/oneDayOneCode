# 📘 課題：ログファイルから特定レベルの行だけを抽出するプログラムを作れ

## 🔧 概要
テキストファイル `app.log` には以下のように、ログレベル付きのメッセージが1行ずつ記録されている。

```log
[INFO] Application started
[DEBUG] Initializing modules
[ERROR] Failed to load config
[INFO] Ready
```

本課題では、**ログファイルを読み込み、指定したログレベルの行だけを抽出して標準出力に表示する**プログラムを作成せよ。

---

## 🎯 要件

1. 入力として以下を受け取る：
   - `argv[1]` : 読み込むログファイル名（例：`app.log`)
   - `argv[2]` : 抽出したいログレベル（例：`INFO`, `DEBUG`, `ERROR`）

2. プログラムはファイルを1行ずつ読み込み、  
   指定したログレベルで始まる行だけを出力する。

3. ファイルが開けなかった場合は  
   `Failed to open file.` と表示して終了すること。

4. ログレベルの大小は区別する（INFO ≠ info）

---

## 💡 実行例

```shell
$ ./logfilter app.log INFO
[INFO] Application started
[INFO] Ready
```
```shell
$ ./logfilter app.log ERROR
[ERROR] Failed to load config
```

---

## 🌱 ヒント
- `<fstream>` の `std::ifstream` を使う。
- 行の先頭を調べるには `r.find(prefix) == 0` が便利。
- 余裕があれば、ログレベルが存在しないときに何も出力しない設計で良い。

---