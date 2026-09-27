# OrderTotalCalculator

## 概要

商品の注文情報から、購入金額の合計を計算するプログラムを作成してください。

今回は `<numeric>` に含まれる `std::accumulate` を使用します。

## 使用する構造体

以下の構造体を定義してください。

struct Item {
    std::string name;
    int price;
    int quantity;
};

- name     : 商品名
- price    : 1個あたりの価格
- quantity : 購入個数

## 入力データ

main関数内で以下のデータを用意してください。

Apple   : 150円 × 3個
Coffee  : 500円 × 2個
Bread   : 200円 × 4個
Milk    : 250円 × 1個

## 課題

`std::accumulate` とラムダ式を使用して、
すべての商品の購入金額を合計してください。

各商品の購入金額は、

price * quantity

で求めます。

for文を使って合計金額を計算してはいけません。

## 実行結果

Total: 2500 yen

## ヒント

`std::accumulate` は数値だけでなく、
構造体を格納した `std::vector` に対しても使用できます。

#include <numeric>

std::accumulate(
    開始イテレータ,
    終了イテレータ,
    初期値,
    ラムダ式
);

ラムダ式では、

現在までの合計値
+
現在のItemの price * quantity

を返すようにしてください。