# StablePrioritySort

## 問題

タスク一覧を「優先度」の高い順に並び替えるプログラムを作成してください。

各タスクは以下の情報を持ちます。

- `name` : タスク名
- `priority` : 優先度

優先度が同じタスクについては、**元の順番を維持**してください。

この条件を満たすため、`std::stable_sort` を使用してください。

## 入力データ

以下のタスクを `std::vector<Task>` に格納してください。

```cpp
{
    {"Compile", 2},
    {"Test", 3},
    {"Deploy", 1},
    {"Lint", 3},
    {"Package", 2}
}
```

## 期待する出力

```text
Test: 3
Lint: 3
Compile: 2
Package: 2
Deploy: 1
```

`Test` と `Lint` はどちらも優先度3ですが、
元々 `Test` が先に存在しているため、その順番を維持します。

同様に `Compile` と `Package` の順番も維持してください。

## 条件

- C++17を使用すること
- `Task` 構造体を作成すること
- `std::vector<Task>` を使用すること
- `std::stable_sort` を使用すること
- 比較条件はラムダ式で記述すること
- 優先度の高い順に並べること

## ビルド例

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
```

## ポイント

`std::sort` と `std::stable_sort` の違いを確認してください。

`std::stable_sort` では、比較上「同じ順位」と判断された要素について、
並び替え前の順序が維持されます。

今回の比較条件は、例えば次のようになります。

```cpp
a.priority > b.priority
```

優先度が等しい要素について追加の比較条件を書かなくても、
`std::stable_sort` が元の順番を維持します。