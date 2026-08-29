# 今日のC++課題：ムーブ専用クラス `Buffer` を作る

## 難易度
★★★☆☆

## 目安時間
15〜25分

## 使用する主な機能
- コンストラクタ / デストラクタ
- `new[]` / `delete[]`
- ムーブコンストラクタ
- ムーブ代入演算子
- `std::move`
- コピー操作の禁止

---

## 問題

動的に確保した `int` 配列を管理する `Buffer` クラスを作成してください。

`Buffer` は内部で

- `int* data_`
- `std::size_t size_`

を保持します。

## 必須仕様

### 1. コンストラクタ

```cpp
explicit Buffer(std::size_t size);
```

`size` 個の `int` を格納できる配列を `new[]` で確保してください。

配列の各要素は `0` で初期化してください。

---

### 2. デストラクタ

```cpp
~Buffer();
```

確保した配列を `delete[]` で解放してください。

---

### 3. コピー禁止

以下を禁止してください。

```cpp
Buffer(const Buffer&) = delete;
Buffer& operator=(const Buffer&) = delete;
```

---

### 4. ムーブコンストラクタ

```cpp
Buffer(Buffer&& other) noexcept;
```

`other` が所有している配列を、新しい `Buffer` に移してください。

ムーブ後の `other` は、

```text
data_ == nullptr
size_ == 0
```

となるようにしてください。

---

### 5. ムーブ代入演算子

```cpp
Buffer& operator=(Buffer&& other) noexcept;
```

現在所有している配列を適切に解放したうえで、
`other` の配列を受け取ってください。

自己ムーブ代入にも対応してください。

---

### 6. 要素アクセス

以下を実装してください。

```cpp
int& operator[](std::size_t index);
const int& operator[](std::size_t index) const;
```

今回は範囲チェックは不要です。

---

### 7. サイズ取得

```cpp
std::size_t size() const;
```

---

## 動作確認

以下の `main()` が動作するようにしてください。

```cpp
int main() {
    Buffer a(3);

    a[0] = 10;
    a[1] = 20;
    a[2] = 30;

    Buffer b(std::move(a));

    std::cout << "a.size(): " << a.size() << '\n';
    std::cout << "b.size(): " << b.size() << '\n';

    for (std::size_t i = 0; i < b.size(); ++i) {
        std::cout << b[i] << '\n';
    }

    Buffer c(5);
    c = std::move(b);

    std::cout << "b.size(): " << b.size() << '\n';
    std::cout << "c.size(): " << c.size() << '\n';

    return 0;
}
```

## 出力例

```text
a.size(): 0
b.size(): 3
10
20
30
b.size(): 0
c.size(): 3
```

---

## コンパイル例

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
```

## ポイント

今回の目的は、

「ポインタそのものをコピーする」のではなく、
「ポインタが指しているリソースの所有権を別のオブジェクトへ移す」

というムーブセマンティクスの感覚を掴むことです。

特にムーブ代入演算子では、

1. 自分が現在持っているリソース
2. 相手から受け取るリソース
3. ムーブ後の相手の状態

の3点を意識してください。

`std::unique_ptr` を使えばもっと簡単に書けますが、
今回はあえて生ポインタで実装して、
`unique_ptr` が内部で解決してくれている問題を体験するのが狙いです。