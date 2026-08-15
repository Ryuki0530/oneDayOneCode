# 今日のC++課題：`std::transform`による一括変換

## 問題

整数を格納した次の `std::vector<int>` があります。

```cpp
std::vector<int> values = {1, 2, 3, 4, 5};
```

`std::transform` を使って、各要素を2乗した新しい `std::vector<int>` を作成してください。

## 期待する出力

```text
1 4 9 16 25
```

## 条件

- C++17を使用する
- 元の `values` は変更しない
- `for` 文および範囲for文を使わない
- ラムダ式で各要素を2乗する
- 結果の表示には `std::copy` と `std::ostream_iterator` を使用する

## 使用する主なヘッダー

```cpp
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
```

## ヒント

`std::transform` は、指定範囲の各要素に変換処理を適用します。

```cpp
std::transform(
    入力範囲の先頭,
    入力範囲の末尾,
    出力先の先頭,
    変換処理
);
```

出力用の `std::vector<int>` は、あらかじめ入力と同じ要素数で作成してください。

## ビルドコマンド

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
```