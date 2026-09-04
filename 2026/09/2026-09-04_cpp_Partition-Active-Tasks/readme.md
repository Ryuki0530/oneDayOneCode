# PartitionActiveTasks

## 問題

以下の `Task` 構造体があります。

```cpp
struct Task {
    std::string name;
    bool completed;
};
```

次のタスク一覧を `std::vector<Task>` で管理します。

```cpp
{
    {"Compile", true},
    {"Test", false},
    {"Lint", true},
    {"Deploy", false},
    {"Package", true}
}
```

`std::partition` を使用して、

- 未完了タスク (`completed == false`)
- 完了タスク (`completed == true`)

の順番になるように並び替えてください。

その後、以下の形式で表示してください。

## 出力例

```text
Incomplete:
Test
Deploy

Completed:
Compile
Lint
Package
```

## 条件

- C++17を使用する
- `std::partition` を使用する
- `std::sort` は使用しない
- `std::partition` の戻り値を利用して、未完了タスクと完了タスクの境界を判断する

## 補足

`std::partition` は条件を満たす要素を前方、満たさない要素を後方にまとめます。

```cpp
auto middle = std::partition(begin, end, predicate);
```

戻り値 `middle` は、後半グループの先頭を指すイテレータです。

なお、`std::partition` は各グループ内の元の順番を保持する保証がありません。