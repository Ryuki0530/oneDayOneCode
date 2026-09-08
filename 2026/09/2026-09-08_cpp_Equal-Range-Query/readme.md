# EqualRangeQuery

## 問題

整数が格納された `std::vector<int>` と、検索したい整数 `target` が与えられます。

`std::equal_range` を使用して、`target` が vector 内に何個存在するかを求めてください。

また、`target` が存在する場合は、

- 最初に現れるインデックス
- 最後に現れるインデックス
- 出現回数

を表示してください。

`target` が存在しない場合は、

`Not found`

と表示してください。

## 条件

- `std::vector<int>` を使用すること
- `std::sort` を使用して、検索前に vector を昇順に並べること
- `std::equal_range` を使用すること
- 要素を1つずつ走査して `target` を探してはいけない
- C++17で実装すること

## 入力例

```cpp
std::vector<int> values{
    5, 2, 8, 5, 1, 5, 3, 8, 8, 4
};

int target = 8;
```

## ソート後

```text
1 2 3 4 5 5 5 8 8 8
```

## 出力例

```text
First index: 7
Last index: 9
Count: 3
```

## target が存在しない場合

例えば、

```cpp
int target = 6;
```

の場合、

```text
Not found
```

と表示してください。

## ヒント

`std::equal_range` は、

```cpp
auto range = std::equal_range(...);
```

のように使用すると、

- `range.first`
- `range.second`

という2つのイテレータを返します。

`range.first` は `target` と等しい要素の範囲の先頭、
`range.second` はその範囲の「1つ後ろ」を指します。

イテレータ間の距離や、vector の先頭からの距離を求める方法を考えてみてください。