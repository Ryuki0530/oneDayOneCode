# GroupWordsByLength

## 問題

文字列のリストを、文字列の長さごとにグループ分けしてください。

以下のリストを使用します。

["apple", "cat", "banana", "dog", "pen", "orange"]

`Map<Integer, List<String>>` を使い、

- キー: 文字列の長さ
- 値: その長さの文字列一覧

となるように分類してください。

## 条件

- `HashMap` を使用すること
- `computeIfAbsent()` を使用すること
- 元の文字列の順序は維持すること

## 出力例

3: [cat, dog, pen]
5: [apple]
6: [banana, orange]

## 使用する主なクラス・メソッド

- `Map`
- `HashMap`
- `List`
- `ArrayList`
- `computeIfAbsent()`

## 目安時間

5〜10分