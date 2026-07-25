# OneDayOneCode
## 2026-07-25
## C++ : File Statistics with std::filesystem

### 問題

指定したディレクトリを走査し、以下の情報を表示するプログラムを作成してください。

### 入力

コマンドライン引数

```
./main <directory>
```

### 出力例

```
Directory : ./sample

Files      : 12
Directories: 3
Total Size : 145832 bytes

Largest File
------------
path : ./sample/data/log.txt
size : 45218 bytes
```

### 条件

- `std::filesystem` を使用すること
- `std::filesystem::directory_iterator` を使用すること
- サブディレクトリの中までは探索しなくてよい
- 通常ファイルのみサイズを集計する
- ディレクトリ数も数える
- 最大サイズのファイルを求める

### エラー処理

以下の場合はエラーメッセージを表示すること。

- 引数不足
- ディレクトリが存在しない
- ディレクトリではない

### 使用してよいヘッダ

```cpp
#include <filesystem>
#include <iostream>
#include <string>
#include <optional>
```

---

## 発展課題

`std::filesystem::recursive_directory_iterator`
を使い、サブディレクトリも含めて集計してください。

さらに拡張子ごとのファイル数を表示してください。

例

```
.cpp : 15
.hpp : 12
.txt : 8
.md  : 3
```

---

## 学べること

- C++17 std::filesystem
- directory_iterator
- recursive_directory_iterator
- pathクラス
- file_size()
- exists()
- is_directory()
- directory_entry