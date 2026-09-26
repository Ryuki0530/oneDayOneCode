# OneDayOneCode: GroupWordsByLength

## 概要

複数の英単語を、文字数ごとにグループ分けするプログラムを作成してください。

今回はJavaの `Map` と `List` の組み合わせに慣れることが目的です。


## 入力データ

以下の単語を使用してください。

    apple
    cat
    banana
    dog
    grape
    orange
    ant


## 課題

単語を文字数ごとに分類し、

    Map<Integer, List<String>>

に格納してください。

キーには「文字数」、
値には「その文字数を持つ単語の一覧」を格納します。


## 出力例

    3: [cat, dog, ant]
    5: [apple, grape]
    6: [banana, orange]


## 条件

- Map<Integer, List<String>> を使用する
- 単語の文字数は String.length() で取得する
- 同じ文字数の単語は同じListへ格納する
- 最後にMapの内容を出力する


## ヒント

Mapにキーがまだ存在しない場合に、
新しい値を作成できるメソッドがあります。

    computeIfAbsent()

例えば、

    map.computeIfAbsent(key, k -> new ArrayList<>());

とすると、keyが存在しない場合だけ新しいArrayListが作成されます。


## 完成条件

次のように単語が文字数ごとに分類されれば完成です。

    3 -> cat, dog, ant
    5 -> apple, grape
    6 -> banana, orange