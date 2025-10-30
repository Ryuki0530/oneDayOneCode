# 🧩 今日の課題：文字列中の単語を辞書順に並べ替えるプログラム

## 📘 問題文

標準入力から1行の英文文字列を読み込み、  
その中に含まれる単語を **辞書順 (アルファベット順)** に並べ替えて出力してください。

- 単語は **半角英字 (A–Z, a–z)** のみで構成されているものとします。
- 大文字・小文字の区別はせず、すべて小文字として扱います。
- 出力は単語をスペース区切りで1行に出力してください。

---

## 💡 入力形式
```
This is a simple Example
```

## 💡 出力形式
```
a example is simple this
```

---

## 🔍 条件
- `std::transform` を用いて小文字化すること。
- `std::sort` を用いてソートを行うこと。
- 文字列処理には `std::istringstream` を使用しても良い。

---

## 💬 ヒント
```cpp
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>
```

---

## 🧠 追加課題（余裕がある人向け）

句読点（例: `, . ! ?`）を削除してから単語を並べ替えるように拡張してみましょう。

---

### 出力確認サンプル
```
入力:  Programming in C++ Is FUN
出力:  c++ fun in is programming
```
