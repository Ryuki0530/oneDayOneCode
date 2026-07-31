# OneDayOneCode（C++）

## 今日のテーマ
`std::filesystem` を使ったディレクトリ内ファイル一覧

### 問題

指定したディレクトリ内のファイルを一覧表示するプログラムを作成してください。

### 条件

- `std::filesystem` を使用すること
- カレントディレクトリ（`.`）を対象とする
- 通常ファイルのみ表示する（ディレクトリは除く）
- 表示形式は以下とする

```
main.cpp
CMakeLists.txt
README.md
```

### 余力があれば

ファイルサイズ(Byte)も表示してください。

例

```
main.cpp (3251 bytes)
README.md (812 bytes)
```

---

### 使用する主な機能

- `std::filesystem::directory_iterator`
- `std::filesystem::is_regular_file`
- `std::filesystem::path`
- `std::filesystem::file_size`

---

### 学習ポイント

- C++17で追加された `std::filesystem`
- パス操作を文字列ではなく `path` オブジェクトで扱う方法
- ファイル情報の取得方法