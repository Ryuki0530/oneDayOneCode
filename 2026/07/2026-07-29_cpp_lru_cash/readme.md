# OneDayOneCode（C++）

## 今日のテーマ
LRUキャッシュを実装する

## 問題

最大3件までデータを保持できるキャッシュクラスを作成してください。

クラス名：
LRUCache

### メンバ関数

void put(const std::string& key, int value);

- キーが存在しない場合
  - キャッシュへ追加する
- キーが既に存在する場合
  - 値を更新する
- 容量を超える場合
  - 最も長い間使われていない要素を削除する

std::optional<int> get(const std::string& key);

- キーが存在する場合
  - 値を返す
  - その要素を「最近使用した」として扱う
- 存在しない場合
  - std::nullopt を返す

void print() const;

現在のキャッシュ内容を表示する。

例：

put("A", 10)
put("B", 20)
put("C", 30)

get("A")

put("D", 40)

このとき B が削除される。

期待される内容

A = 10
C = 30
D = 40

※表示順は自由

---

## 条件

- std::unordered_map を使用すること
- 「使用順」は std::list を利用して管理すること
- list::splice を使うと要素移動が高速に行える
- 容量は constexpr size_t CAPACITY = 3; とする

---

## 発展課題（時間があれば）

LRUCache をテンプレート化し、

template<typename Key, typename Value>

で任意のキー・値型に対応させてください。