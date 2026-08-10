# 今日のC++課題：`std::from_chars`で整数を安全に変換

## 目的

例外を使わず、高速に文字列から整数へ変換できる  
C++17の`std::from_chars`を使ってみましょう。

## 問題

次の文字列を順番に整数へ変換してください。

- `"123"`
- `"-45"`
- `"12abc"`
- `"999999999999999999999"`

変換結果に応じて、以下のように表示してください。

- 文字列全体を変換できた場合：`成功: 数値`
- 一部だけ変換できた場合：`途中まで成功: 数値`
- 数値として解釈できない場合：`変換失敗`
- `int`の範囲を超えた場合：`範囲外`

## 条件

- C++17を使用する
- `std::stoi`や`std::stringstream`は禁止
- 変換処理を次の関数にまとめる

    void parseInteger(std::string_view text);

## 実行例

成功: 123
成功: -45
途中まで成功: 12
範囲外

## ヒント

必要なヘッダー：

    #include <charconv>
    #include <string_view>
    #include <system_error>

`std::from_chars`の戻り値には次の情報があります。

- `ptr`：変換が終了した位置
- `ec`：変換結果を示すエラーコード

文字列全体を変換できたかどうかは、次の比較で判断できます。

    result.ptr == text.data() + text.size()

## ビルド

    g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o main