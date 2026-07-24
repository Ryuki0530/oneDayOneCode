# OneDayOneCode

## タイトル
**Span-Based Statistics**

## 問題
`std::span<int>` を受け取る関数を作成し、以下を返してください。

```cpp
struct Stats {
    int min;
    int max;
    double average;
};
```

関数

```cpp
Stats calculate(std::span<const int> values);
```

を実装してください。

### 条件
- `std::vector<int>`、`std::array<int, N>`、C配列のすべてから呼び出せること
- 配列をコピーしないこと
- `std::span` を利用すること
- 要素数は1以上とする

### 動作例

```cpp
std::vector<int> v{3, 8, 1, 5};

auto s = calculate(v);

min = 1
max = 8
average = 4.25
```

## 学習ポイント
- `std::span`
- コピーを伴わない配列受け渡し
- rangeベースfor