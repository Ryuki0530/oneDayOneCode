# 今日のC++課題：`std::map::extract()` でキーを書き換える

## 目的

C++17で追加された `std::map::extract()` と node handle の使い方を学ぶ。

## 問題

次のようなユーザーIDと名前を管理する `std::map` がある。

```cpp
std::map<int, std::string> users = {
    {100, "Alice"},
    {200, "Bob"},
    {300, "Charlie"}
};
```

ユーザーID `200` を `250` に変更したい。

ただし、以下の方法は禁止とする。

```cpp
users[250] = users[200];
users.erase(200);
```

`std::map::extract()` を使用して、
既存要素を取り出し、そのキーを変更して再挿入すること。

## 要件

1. `std::map<int, std::string>` を使用する。
2. `users.extract(200)` で要素を取り出す。
3. node handle の `key()` を使ってキーを `250` に変更する。
4. `insert()` で元の `map` に戻す。
5. 最後に全要素を表示する。

## 期待する出力

```text
100 : Alice
250 : Bob
300 : Charlie
```

## 使用するもの

- `std::map`
- `std::map::extract`
- node handle
- `node.key()`
- `std::map::insert`

## コンパイル

```bash
g++ -std=c++17 main.cpp -o main
./main
```

## 余裕があれば

変更先のキー `250` がすでに存在していた場合に、
ID変更を行わないようにしてみる。