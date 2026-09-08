# TopNResults

## 概要

複数のテスト結果から、得点が高い上位3件だけを取得してください。

今回は `std::partial_sort_copy` を使用します。

元の `results` の並び順は変更してはいけません。

## 条件

以下の構造体を使用してください。

```cpp
struct Result {
    std::string name;
    int score;
};
```

次のデータを用意します。

```cpp
std::vector<Result> results{
    {"Alice", 78},
    {"Bob", 92},
    {"Charlie", 85},
    {"Dave", 67},
    {"Eve", 95},
    {"Frank", 88}
};
```

`std::partial_sort_copy` を使用して、得点の高い順に上位3件を取得してください。

## 出力例

```text
Eve: 95
Bob: 92
Frank: 88
```

## 制約

- C++17以降
- `std::sort` は使用しない
- 元の `results` の順序を変更しない
- `std::partial_sort_copy` を使用する
- ラムダ式を使って得点の降順を指定する

## ファイル構成

```text
TopNResults/
└── main.cpp
```

## ヒント

`std::partial_sort_copy` は次のように使用します。

```cpp
std::partial_sort_copy(
    input_begin,
    input_end,
    output_begin,
    output_end,
    compare
);
```

入力範囲のデータを並べたと仮定したとき、その先頭部分だけを別の範囲へコピーできます。

今回であれば、まず上位3件を保存する領域を用意します。

```cpp
std::vector<Result> top_results(3);
```

そして、

```text
results
   ↓
std::partial_sort_copy
   ↓
top_results
```

という形で上位3件だけを取り出してください。

`std::partial_sort` と異なり、入力元の `results` 自体を並べ替えない点がポイントです。