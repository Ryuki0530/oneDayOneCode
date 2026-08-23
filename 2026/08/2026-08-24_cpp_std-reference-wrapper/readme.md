# 今日のC++課題：`std::reference_wrapper` でオブジェクトの参照をコンテナに保存する

## 概要

C++の参照型 `T&` は、そのまま `std::vector` の要素型にはできません。

例えば、次のコードは使用できません。

```cpp
std::vector<Device&> devices;
```

そこで今回は `std::reference_wrapper` と `std::ref()` を使い、
既存オブジェクトへの参照を `std::vector` に保存するプログラムを作成してください。

---

## 要件

次のクラスを作成してください。

```cpp
class Device
{
public:
    Device(std::string name, bool enabled);

    void enable();
    void disable();

    bool isEnabled() const;
    const std::string& getName() const;

private:
    std::string name_;
    bool enabled_;
};
```

`main()` で次の3つのオブジェクトを作成してください。

```text
Camera : enabled
LiDAR  : disabled
Motor  : enabled
```

その後、

```cpp
std::vector<std::reference_wrapper<Device>>
```

を使って、3つの `Device` への参照を1つのvectorに格納してください。

---

## 処理

vectorを順番に走査し、

- enabled のDevice → `disable()`
- disabled のDevice → `enable()`

として、すべての状態を反転してください。

最後に各Deviceの状態を表示してください。

---

## 実行結果例

```text
Camera : disabled
LiDAR : enabled
Motor : disabled
```

---

## 制約

- C++17を使用すること
- `std::vector<std::reference_wrapper<Device>>` を使用すること
- `Device` オブジェクトそのものをvectorへコピーしないこと
- `Device*` は使用しないこと
- `std::ref()` を使用してvectorへ登録すること

---

## コンパイル例

```powershell
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
./main
```

---

## 考えてほしいポイント

次のコードについて、

```cpp
Device camera("Camera", true);

std::reference_wrapper<Device> ref = std::ref(camera);

ref.get().disable();
```

なぜ `camera` 自体も `disabled` になるのか考えてみてください。

また、

```cpp
std::vector<Device>
```

にオブジェクトを格納する場合との違いについても考えてみてください。