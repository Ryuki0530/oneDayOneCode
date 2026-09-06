# PartitionByScore

## 問題

学生の名前と点数を保持する `Student` 構造体があります。

点数が **60点以上の学生を合格、それ未満を不合格** として、
`std::partition` を使って `std::vector<Student>` を2つのグループに分けてください。

## 条件

- C++17 を使用する
- `std::partition` を使用する
- 新しい `vector` を作って分類してはいけない
- 元の `students` の中で直接並び替えること
- `std::partition` の戻り値を利用して、合格者と不合格者をそれぞれ表示すること

## 初期データ

```cpp
std::vector<Student> students{
    {"Alice", 82},
    {"Bob", 45},
    {"Charlie", 71},
    {"Dave", 58},
    {"Eve", 90}
};
```

## Student

```cpp
struct Student {
    std::string name;
    int score;
};
```

## 出力例

`std::partition` は要素の順番を保証しないため、以下は一例です。

```text
Passed:
Alice: 82
Charlie: 71
Eve: 90

Failed:
Dave: 58
Bob: 45
```

## ヒント

`std::partition` は次のように使用できます。

```cpp
auto boundary = std::partition(
    students.begin(),
    students.end(),
    [](const Student& student) {
        return /* 条件 */;
    }
);
```

`boundary` は、条件を満たすグループの直後を指すイテレータです。

そのため、

```text
students.begin() ～ boundary
```

が条件を満たす要素、

```text
boundary ～ students.end()
```

が条件を満たさない要素になります。

## ゴール

以下を理解してください。

- `std::partition` の使い方
- ラムダ式による分類条件の指定
- `std::partition` の戻り値の意味
- イテレータを使った範囲の分割
- `std::partition` は要素の元の順番を保証しないこと

## ファイル名

main.cpp

## ビルド例

```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o main
```