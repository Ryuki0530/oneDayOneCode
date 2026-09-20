# TotalPriceCalculator

## 問題

商品の価格一覧が `std::vector<int>` で与えられています。

`std::accumulate` を使用して、価格の合計を計算してください。

ただし、価格が `1000円以上` の商品だけを合計対象としてください。

## 条件

- C++17を使用する
- `std::accumulate` を使用する
- ラムダ式を使用する
- 通常の `for` 文による合計処理は禁止

## 入力例

```cpp
std::vector<int> prices{
    500,
    1200,
    3000,
    800,
    1500
};
```

## 期待する出力

```text
Total: 5700
```

## ヒント

`std::accumulate` の第4引数には、加算方法を指定する関数やラムダ式を渡すことができます。

```cpp
std::accumulate(begin, end, initial_value, lambda);
```

ラムダ式の中で、現在の商品価格が1000円以上かどうかを判定してみてください。