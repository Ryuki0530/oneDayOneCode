# One Day One Code

## タイトル
Result Parser

## 難易度
★★☆☆☆

## 想定時間
15〜20分

## 問題

文字列を受け取り、整数へ変換する関数を実装してください。

関数仕様

parseInt(std::string)

成功時
- 変換後の整数を返す

失敗時
- エラー内容を返す

エラーは最低でも以下の2種類を区別してください。

- Empty Input
- Invalid Number

## 入力例

"123"
""
"abc"
"-42"

## 出力例

Success: 123
Error: Empty Input
Error: Invalid Number
Success: -42

## 条件

- atoiは禁止
- std::stoiは使用可
- 例外は関数内部で処理し、呼び出し側には成功/失敗だけを返す
- 呼び出し側では例外処理を書かない

## 発展課題（時間があれば）

ErrorCode を enum class に変更する。

例

enum class ParseError {
    EmptyInput,
    InvalidNumber
};

文字列ではなく ErrorCode を返す設計にしてみましょう。