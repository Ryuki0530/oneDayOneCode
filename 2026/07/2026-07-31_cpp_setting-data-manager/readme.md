# OneDayOneCode（C++）

## 問題：設定ファイル管理クラス

設定ファイルを読み込んだことを想定したクラスを実装してください。

### 仕様

`Config`クラスを作成する。

内部では

```cpp
std::unordered_map<std::string, std::string>
```

で値を保持する。

---

### 実装する関数

#### 1.

```cpp
void set(const std::string& key, const std::string& value);
```

設定を追加・更新する。

---

#### 2.

```cpp
std::optional<std::string> get(const std::string& key) const;
```

キーが存在すれば値を返す。

存在しなければ

```cpp
std::nullopt
```

を返す。

---

#### 3.

```cpp
template<typename T>
std::optional<T> getAs(const std::string& key) const;
```

以下の型だけ対応する。

- int
- double
- bool
- std::string

例

```cpp
config.set("port", "8080");
config.set("pi", "3.14");
config.set("debug", "true");
```

↓

```cpp
auto port = config.getAs<int>("port");
auto pi = config.getAs<double>("pi");
auto debug = config.getAs<bool>("debug");
```

変換に失敗したら

```cpp
std::nullopt
```

を返すこと。

---

### 動作例

```text
Port : 8080
Pi   : 3.14
Debug: true

timeout is not found
```

---

### 制約

- 例外は投げない
- `std::optional`を活用する
- 型変換は `std::stoi`、`std::stod` などを利用してよい
- boolは `"true"` と `"false"` のみ受け付ける

---

## 発展課題（余裕があれば）

```cpp
bool load(const std::string& filename);
```

を追加し、

```
port=8080
host=localhost
debug=true
```

のようなファイルを読み込めるようにする。

コメント行（#で始まる）は無視してよい。