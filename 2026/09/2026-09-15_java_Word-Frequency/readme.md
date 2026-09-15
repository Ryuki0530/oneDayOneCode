# Java Daily Challenge: WordFrequency

## 問題

文字列のリストから、各文字列の出現回数を集計するプログラムを作成してください。

以下のデータを使用します。

```java
List<String> words = List.of(
    "apple",
    "banana",
    "apple",
    "orange",
    "banana",
    "apple"
);
```

`HashMap<String, Integer>` を使用して、各単語の出現回数を記録してください。

## 出力例

```text
apple: 3
banana: 2
orange: 1
```

※ 出力順序は問いません。

## 条件

- Java 17以降
- `HashMap` を使用すること
- 同じ単語が登場した場合はカウントを1増やすこと
- `getOrDefault()` を使用して実装すること

## ヒント

`HashMap` から値を取得するとき、

```java
map.getOrDefault(key, 0)
```

とすると、`key` が存在しない場合に `0` を取得できます。

## 発展

余裕があれば、最も出現回数の多い単語も最後に表示してください。

```text
Most frequent: apple
```