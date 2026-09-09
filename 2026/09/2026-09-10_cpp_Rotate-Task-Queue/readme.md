# RotateTaskQueue

## 問題

以下のタスク一覧があります。

```cpp
struct Task {
    std::string name;
    bool active;
};

std::vector<Task> tasks{
    {"Compile", false},
    {"Test", false},
    {"Deploy", true},
    {"Monitor", false},
    {"Cleanup", false}
};
```

`active == true` のタスクを先頭に移動し、
それより前にあった要素を末尾へ回してください。

ただし、各要素の相対的な順番は維持してください。

## 期待する出力

```text
Deploy
Monitor
Cleanup
Compile
Test
```

## 条件

- C++17を使用する
- `std::find_if` を使用して `active == true` の要素を探す
- `std::rotate` を使用して並び替える
- 新しい `std::vector` を作ってコピーする方法は禁止
- `active == true` の要素が存在しない場合は何もしない

## 使用する主なヘッダ

```cpp
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
```

## ヒント

`std::rotate` は次のように使用できます。

```cpp
std::rotate(first, middle, last);
```

`middle` が新しい先頭になります。

例えば、

```text
A B C D E
```

に対して `C` を `middle` に指定すると、

```text
C D E A B
```

となります。

今回のポイントは、

```cpp
std::find_if(...)
std::rotate(...)
```

を組み合わせることです。

特に `std::rotate(first, middle, last)` の
「middle が新しい先頭になる」
という感覚を掴んでください。