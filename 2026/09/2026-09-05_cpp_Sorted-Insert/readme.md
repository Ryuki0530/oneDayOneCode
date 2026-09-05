# SortedInsert

## 問題

整数を昇順に保持する `SortedNumbers` クラスを作成してください。

内部では `std::vector<int>` を使用します。

## 要件

以下のメンバ関数を実装してください。

~~~cpp
void insert(int value);
~~~

`insert()` は、新しい値を追加した後も `std::vector` が昇順になる位置へ値を挿入します。

ただし、挿入位置の探索には以下を使用してください。

~~~cpp
std::lower_bound
~~~

同じ値がすでに存在する場合も、その値を追加してください。

---

## 使用例

~~~cpp
int main() {
    SortedNumbers numbers;

    numbers.insert(30);
    numbers.insert(10);
    numbers.insert(20);
    numbers.insert(20);
    numbers.insert(5);

    numbers.print();
}
~~~

## 期待される出力

~~~text
5 10 20 20 30
~~~

## 制約

- 内部データは `std::vector<int>` を使用する
- `insert()` の挿入位置探索に `std::lower_bound` を使用する
- `std::sort` を使用してはいけない
- `print()` で現在の値を昇順に表示する

## ヒント

`std::lower_bound(begin, end, value)` は、

「`value` 以上の値が最初に現れる位置」

を示すイテレータを返します。

返されたイテレータは、そのまま `std::vector::insert()` の挿入位置として利用できます。