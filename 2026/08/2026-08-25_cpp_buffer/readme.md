# 今日のC++課題：動的配列を管理する `Buffer` クラス

## 目的

C++の以下の要素を練習する。

- RAII
- コピーコンストラクタ
- コピー代入演算子
- ムーブコンストラクタ
- ムーブ代入演算子
- Rule of Five
- Deep Copy（深いコピー）
- 所有権の移動

---

## 問題

整数配列を動的に確保して管理する `Buffer` クラスを実装してください。

`std::vector` や `std::unique_ptr` は使用せず、
内部では `new[]` / `delete[]` を使用してください。

---

## Buffer クラス

以下のメンバを持つものとします。

```cpp
class Buffer {
private:
    int* data_;
    std::size_t size_;

public:
    // TODO
};
```

次の機能を実装してください。

### 1. コンストラクタ

```cpp
Buffer(std::size_t size);
```

指定された要素数の `int` 配列を動的確保する。

すべての要素を `0` で初期化すること。

---

### 2. デストラクタ

確保した配列を解放する。

---

### 3. コピーコンストラクタ

```cpp
Buffer(const Buffer& other);
```

`other` のデータを Deep Copy すること。

つまり、

```cpp
Buffer a(3);
Buffer b = a;
```

とした場合、

`a.data_` と `b.data_` は別々のメモリを指していなければならない。

---

### 4. コピー代入演算子

```cpp
Buffer& operator=(const Buffer& other);
```

次のケースでも正しく動作すること。

```cpp
Buffer a(3);
Buffer b(10);

b = a;
```

また、

```cpp
a = a;
```

という自己代入でも壊れないようにすること。

---

### 5. ムーブコンストラクタ

```cpp
Buffer(Buffer&& other) noexcept;
```

`other` が持っている配列の所有権を受け取る。

ムーブ後の `other` は、

```cpp
other.data_ == nullptr
other.size_ == 0
```

となるようにすること。

---

### 6. ムーブ代入演算子

```cpp
Buffer& operator=(Buffer&& other) noexcept;
```

現在所有しているメモリを適切に解放してから、
`other` の所有権を受け取ること。

---

### 7. 要素アクセス

以下を実装する。

```cpp
int& operator[](std::size_t index);
const int& operator[](std::size_t index) const;
```

今回は範囲チェックは不要。

---

### 8. size()

```cpp
std::size_t size() const;
```

要素数を返す。

---

## 動作確認

以下の `main()` を使用してください。

```cpp
#include <cstddef>
#include <iostream>
#include <utility>

int main()
{
    Buffer a(3);

    a[0] = 10;
    a[1] = 20;
    a[2] = 30;

    // コピー
    Buffer b = a;

    b[0] = 999;

    std::cout << "a[0] = " << a[0] << '\n';
    std::cout << "b[0] = " << b[0] << '\n';

    // コピー代入
    Buffer c(1);
    c = a;

    std::cout << "c[1] = " << c[1] << '\n';

    // ムーブ
    Buffer d = std::move(a);

    std::cout << "d size = " << d.size() << '\n';
    std::cout << "a size = " << a.size() << '\n';

    // ムーブ代入
    Buffer e(100);
    e = std::move(b);

    std::cout << "e[0] = " << e[0] << '\n';
    std::cout << "b size = " << b.size() << '\n';
}
```

---

## 期待される出力

```text
a[0] = 10
b[0] = 999
c[1] = 20
d size = 3
a size = 0
e[0] = 999
b size = 0
```

---

## 制約

- C++17
- `std::vector` 使用禁止
- `std::unique_ptr` 使用禁止
- `std::shared_ptr` 使用禁止
- 配列は `new[]` で確保する
- 配列は `delete[]` で解放する
- メモリリークを発生させない
- コピーでは Deep Copy を行う
- ムーブでは新しい配列を確保しない

---

## ビルド

Windows / MinGW:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
./main
```

---

## 考えてほしいポイント

特に次の違いを意識してください。

```cpp
Buffer b = a;
```

と

```cpp
Buffer b = std::move(a);
```

では、内部の `int*` に対して何が起きるべきでしょうか？

コピーでは、

「同じデータを持つ別のメモリ」

ムーブでは、

「同じメモリの所有者を入れ替える」

という違いがあります。