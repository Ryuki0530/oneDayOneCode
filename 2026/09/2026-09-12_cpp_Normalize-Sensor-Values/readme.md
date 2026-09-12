# NormalizeSensorValues

## 問題

センサから取得した整数値を格納する `std::vector<int>` があります。

```cpp
std::vector<int> raw_values = {120, 80, 200, 50, 150};
```

これらの値を、基準値 `100` との差分に変換してください。

変換後の値は次の式で求めます。

`変換後の値 = 元の値 - 100`

`std::transform` を使用し、変換結果を別の `std::vector<int>` に格納してください。

## 期待する出力

```text
20
-20
100
-50
50
```

## 条件

- C++17を使用すること
- `std::transform` を使用すること
- ラムダ式を使用すること
- 元の `raw_values` は変更しないこと
- 変換結果は別の `std::vector<int>` に保存すること

## ヒント

`std::transform` は、ある範囲の各要素に処理を適用し、その結果を別の範囲へ書き込めます。

必要になるヘッダ：

```cpp
#include <algorithm>
#include <iostream>
#include <vector>
```