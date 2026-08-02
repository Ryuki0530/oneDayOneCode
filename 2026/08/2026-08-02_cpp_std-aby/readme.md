# 今日の課題（C++）

## テーマ
`std::any` を使った簡易設定管理

## 問題

異なる型の設定値を1つのコンテナで管理する `Config` クラスを作成してください。

### 要件

- `std::unordered_map<std::string, std::any>` を使用する。
- 以下のメンバ関数を実装する。

```cpp
template<typename T>
void set(const std::string& key, T value);

template<typename T>
T get(const std::string& key) const;
```

### 動作例

```text
name = Alice
age = 20
pi = 3.14159
```

### エラー処理

以下の場合は `std::runtime_error` を送出すること。

- 存在しないキー
- 型が一致しない

## 発展（時間があれば）

```cpp
bool contains(const std::string& key) const;
```

```cpp
void remove(const std::string& key);
```

も追加してください。