# OneDayOneCode - C++
# Title: OptionalConfigLoader

## 問題

設定値を取得する簡単なプログラムを作成してください。

### 条件

1. 以下の関数を作成する。

std::optional<int> getConfigValue(const std::string& key);

2. 設定値はプログラム内で保持する。

例

timeout = 30
retry = 5

3. 存在するキーなら値を返す。
4. 存在しないキーなら std::nullopt を返す。

### 実行例

timeout = 30
retry = 5
buffer = Not Found

### 制約

- std::unordered_map を使用すること
- std::optional を使用すること
- if (value.has_value()) または if (value) のどちらかで判定すること

## 学習ポイント

- std::optional
- std::nullopt
- std::unordered_map
- 値が存在しないことを安全に表現する方法