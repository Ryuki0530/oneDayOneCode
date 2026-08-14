# 今日のC++課題：スレッドセーフな設定管理クラス

## 概要

複数のスレッドから設定値を読み書きできる `ConfigStore` クラスを実装してください。

設定の読み取りは複数スレッドから同時に行えるようにし、書き込み中だけ排他的にロックします。

## 使用する機能

- `std::unordered_map`
- `std::shared_mutex`
- `std::shared_lock`
- `std::unique_lock`
- `std::optional`
- `std::thread`

## 要件

`ConfigStore` クラスに、次のメンバ関数を実装してください。

### set

```cpp
void set(const std::string& key, const std::string& value);
```

- 指定されたキーに値を保存する
- 同じキーが存在する場合は上書きする
- 書き込み中は、ほかの読み書きを禁止する
- `std::unique_lock<std::shared_mutex>` を使用する

### get

```cpp
std::optional<std::string> get(const std::string& key) const;
```

- キーが存在する場合は、その値を返す
- 存在しない場合は `std::nullopt` を返す
- 複数のスレッドが同時に読み取れるようにする
- `std::shared_lock<std::shared_mutex>` を使用する

## main関数の動作

1. `ConfigStore` を1つ生成する
2. `mode` に `debug` を設定する
3. 3個の読み取りスレッドを起動する
4. 各スレッドで `mode` の値を表示する
5. すべてのスレッドを `join()` する
6. 存在しないキーも読み取り、存在しないことを表示する

## 実行結果の例

```text
Reader 1: debug
Reader 2: debug
Reader 3: debug
timeout was not found
```

スレッドの実行順序によって、Readerの表示順は変わって構いません。

## 制約

- C++17を使用する
- グローバル変数を使用しない
- `std::mutex` ではなく `std::shared_mutex` を使用する
- `get()` からマップ内部の文字列への参照を返さない
- ロックとアンロックを手動で行わない

## ヒント

`std::mutex` では、読み取りだけの処理同士も直列化されます。

一方、`std::shared_mutex` には2種類のロックがあります。

- `std::shared_lock`：ほかの読み取り処理を許可する
- `std::unique_lock`：読み取りと書き込みの両方を禁止する

`get()` は文字列の参照ではなくコピーを返してください。

ロック解除後に別のスレッドがマップを書き換える可能性があるため、内部データへの参照を外へ出すと安全性を維持できません。

## ビルドコマンド

```bash
g++ -std=c++17 -Wall -Wextra -pedantic -pthread main.cpp -o main
```