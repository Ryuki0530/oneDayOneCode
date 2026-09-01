# Remove Duplicate Elements

## 目標

`std::sort`と`std::unique`を組み合わせて、整数配列から重複した値を削除してください。

## 要件

次のデータを持つ`std::vector<int>`を用意します。

    {4, 2, 7, 2, 4, 9, 1, 7}

以下の処理を実装してください。

1. 要素を昇順に並べ替える
2. 重複している要素を削除する
3. 残った要素を空白区切りで出力する

## 期待する出力

    1 2 4 7 9

## 制約

- C++17を使用する
- `std::set`は使用しない
- `std::sort`と`std::unique`を使用する
- `std::unique`だけではコンテナのサイズが変わらない点に注意する

## ビルド例

    g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main
    ./main