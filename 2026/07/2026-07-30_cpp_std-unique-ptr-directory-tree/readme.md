# 今日の課題：`std::unique_ptr`でディレクトリツリーを作る

## 問題

以下の `Node` クラスを実装してください。

```cpp
class Node {
public:
    explicit Node(std::string name);

    void addChild(std::unique_ptr<Node> child);

    void print(int depth = 0) const;

private:
    std::string name_;
    std::vector<std::unique_ptr<Node>> children_;
};
```

---

## 条件

- `children_` は `std::unique_ptr<Node>` で保持する
- `addChild()` では子ノードの所有権を受け取る
- `print()` では深さに応じてインデントを付けて表示する
- `new` と `delete` は直接使用しない

---

## 期待する出力例

```
root
  home
    user
    guest
  etc
```

---

## main関数の例

```cpp
int main() {
    auto root = std::make_unique<Node>("root");

    auto home = std::make_unique<Node>("home");
    home->addChild(std::make_unique<Node>("user"));
    home->addChild(std::make_unique<Node>("guest"));

    root->addChild(std::move(home));
    root->addChild(std::make_unique<Node>("etc"));

    root->print();

    return 0;
}
```

---

## 必要なヘッダ

```cpp
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
```

---

## コンパイル

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o main
```

---

## 実行

```bash
./main
```

---

# 発展課題

`Node` クラスに次のメンバ関数を追加してください。

```cpp
Node* find(const std::string& name);
```

指定された名前のノードを、自分自身と子ノードから再帰的に探索します。

- 見つかった場合は、その `Node` へのポインタを返す
- 見つからなかった場合は `nullptr` を返す

使用例

```cpp
Node* result = root->find("guest");

if (result != nullptr) {
    std::cout << "Found" << std::endl;
} else {
    std::cout << "Not found" << std::endl;
}
```

---

# 今回のポイント

- `std::unique_ptr` による所有権管理
- `std::make_unique` によるオブジェクト生成
- `std::move` による所有権の移動
- 再帰的なデータ構造
- RAIIによる自動的なメモリ解放