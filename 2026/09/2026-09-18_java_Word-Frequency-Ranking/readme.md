# Daily Java Challenge: WordFrequencyRanking

## 問題

文字列のリストから、各単語の出現回数を数え、
以下のルールで並べ替えて表示するプログラムを作成してください。

## 並び順

1. 出現回数が多い単語を先にする
2. 出現回数が同じ場合は、辞書順（昇順）にする

## 入力データ

main メソッド内で次のリストを使用してください。

List<String> words = List.of(
    "apple",
    "banana",
    "apple",
    "orange",
    "banana",
    "apple",
    "grape",
    "orange"
);

## 期待する出力

apple: 3
banana: 2
orange: 2
grape: 1

## 条件

- HashMap<String, Integer> を使用して出現回数を数えること
- Stream API は使用しないこと
- Collections.sort または List.sort を使用すること
- Comparator を自分で記述すること
- 単語や出現回数を直接決め打ちしてはいけない

## ヒント

まず、

Map<String, Integer>

に

単語 -> 出現回数

という形でデータを保存します。

その後、Map のキーを List に変換して、
Comparator を使って並び替える方法を考えてみてください。

## ファイル名

Main.java