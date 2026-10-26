# Word Wrap

使用言語: Java
目安: 15〜20分

## 目的
単語の途中で分割せず、指定した文字数以内に収まるように英文を改行するプログラムを作成してください。

## 実装要件
1. 次のメソッドを実装してください。
   static List<String> wrapText(String text, int width)

2. 単語を先頭から順番に各行へ追加してください。
   次の単語を追加するとwidth文字を超える場合は、新しい行へ移します。
   単語間には半角スペースを1つだけ入れ、スペースも文字数に含めます。

3. 入力の前後の空白は無視し、連続する半角スペースは区切りとして扱ってください。
   空文字列や半角スペースのみの場合は、空のリストを返してください。

4. widthは1以上とし、各単語の長さはwidth以下とします。
   入力は半角英字と半角スペースのみです。

5. mainメソッドで戻り値を1行ずつ表示してください。

## 入力例
text = "Java makes text processing fun and useful"
width = 16

## 期待出力
Java makes text
processing fun
and useful

## 追加の確認例
text = "  one   two three  "
width = 7

期待出力:
one two
three

## 完了条件
上記2例が期待どおりに表示され、空文字列では何も表示されないこと。
各行の先頭と末尾にスペースが付かないこと。