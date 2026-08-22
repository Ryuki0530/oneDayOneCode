# 今日のC++課題：`std::erase`を使わない要素削除

## 問題

整数を管理する `NumberList` クラスを作成してください。

次の機能を実装します。

- `add(int value)`
  - 数値を末尾へ追加する
- `remove_if_greater_than(int threshold)`
  - `threshold` より大きい要素をすべて削除する
  - C++17の `std::remove_if` と `vector::erase` を組み合わせる
- `print()`
  - 現在の要素を空白区切りで表示する

## 実行例

```cpp
NumberList numbers;

numbers.add(3);
numbers.add(12);
numbers.add(7);
numbers.add(20);
numbers.add(5);

numbers.remove_if_greater_than(10);
numbers.print();
```

## 期待する出力
```bash
3 7 5
```

## 条件
- C++17を使用する
- 内部データには std::vector<int> を使用する
- 手書きループで削除しない
- 「erase-removeイディオム」を使用する

## ビルド例
```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o main
```