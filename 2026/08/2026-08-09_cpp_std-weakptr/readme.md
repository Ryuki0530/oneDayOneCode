# 今日の課題：std::weak_ptr で循環参照を防ぐ

## 目的

`std::shared_ptr` と `std::weak_ptr` の違いを理解し、
「所有するポインタ」と「参照するだけのポインタ」を使い分ける。

## 課題

親子関係を持つ `Node` クラスを作成してください。

Node は以下を持ちます。

- ノード名 `std::string name_`
- 子ノード `std::vector<std::shared_ptr<Node>> children_`
- 親ノード `std::weak_ptr<Node> parent_`

## 実装する機能

### Node::addChild()

子ノードを追加する。

同時に、追加された子ノードの `parent_` に
現在のNodeを設定する。

### Node::printParent()

親Nodeが存在する場合

Parent of child: root

のように表示する。

親Nodeが既に削除されている場合は

Parent expired

と表示する。

## 条件

- Node は `std::enable_shared_from_this<Node>` を継承する
- `addChild()` 内で `shared_from_this()` を使用する
- `parent_` は必ず `std::weak_ptr<Node>` とする
- `weak_ptr::lock()` を使って親Nodeへアクセスする

## main() の動作例

1. `root` Nodeを `std::make_shared` で作成
2. `child` Nodeを作成
3. `root->addChild(child)` で親子関係を作る
4. `child->printParent()` を実行する

期待する出力：

Parent of child: root

## 追加課題

余裕があれば、Nodeのデストラクタで

Destroyed: ノード名

を表示してください。

そして、なぜ `parent_` まで `std::shared_ptr` にすると
rootとchildが破棄されなくなる可能性があるのか考えてください。