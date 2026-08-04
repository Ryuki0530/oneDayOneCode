# 今日のC++課題：スレッドセーフな設定値ストア

## 概要

複数のスレッドから安全に読み書きできる設定値管理クラス `ConfigStore` を実装してください。

設定値は、文字列のキーと整数値の組み合わせで管理します。

## 使用する主な機能

- `std::unordered_map`
- `std::shared_mutex`
- `std::shared_lock`
- `std::unique_lock`
- `std::thread`

## 要件

次のインターフェースを持つ `ConfigStore` クラスを作成してください。

### `void set(const std::string& key, int value)`

指定されたキーに値を設定します。

同じキーがすでに存在する場合は、値を上書きしてください。

### `std::optional<int> get(const std::string& key) const`

指定されたキーの値を取得します。

キーが存在しない場合は `std::nullopt` を返してください。

### `bool remove(const std::string& key)`

指定されたキーを削除します。

削除できた場合は `true`、キーが存在しなかった場合は `false` を返してください。

## 排他制御

- `set()` と `remove()` はデータを書き換えるため、排他的にロックしてください。
- `get()` は読み取り専用なので、複数のスレッドが同時に実行できるようにしてください。
- ミューテックスは `mutable std::shared_mutex` としてクラス内に保持してください。

## 動作確認

`main()` で次の処理を実装してください。

1. `ConfigStore` を1つ作成する。
2. 書き込みスレッドを1つ起動し、`volume` に `0` から `9` まで順番に設定する。
3. 読み取りスレッドを2つ起動し、`volume` の値を繰り返し取得して表示する。
4. すべてのスレッドを `join()` する。
5. 最後に `volume` の値を表示する。

各ループには `std::this_thread::sleep_for()` を入れ、スレッドが並行して動いていることを確認しやすくしてください。

## 実行例

出力順序はスレッドの実行タイミングによって変化します。

Reader 1: volume = 0
Reader 2: volume = 0
Reader 1: volume = 2
Reader 2: volume = 3
Reader 1: volume = 5
Reader 2: volume = 6
Final value: 9

## 制約

- C++17以上を使用すること。
- グローバル変数を使用しないこと。
- `std::mutex` ではなく `std::shared_mutex` を使用すること。
- ロックとアンロックを手動で行わず、RAII形式のロッククラスを使用すること。

## ビルドコマンド

### Windows（MinGW）

g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main

### WSL / Linux

g++ -std=c++17 -Wall -Wextra -pedantic -pthread main.cpp -o main