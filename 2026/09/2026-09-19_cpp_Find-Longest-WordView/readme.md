# FindLongestWordView

## 問題

C++17 の `std::string_view` を使って、文章中から最も長い単語を取得する関数を実装してください。

次の関数を作成してください。

```cpp
std::string_view findLongestWord(std::string_view text);
```

文章中の単語は半角スペース `' '` で区切られているものとします。

最も長い単語が複数存在する場合は、最初に登場した単語を返してください。

## 条件

- C++17 を使用する
- `std::string_view` を使用する
- 単語を取り出すために `std::string` を新しく生成しない
- `substr()` を使用してよい
- 入力文字列は変更しない

## 入力例

```cpp
std::string text = "CAN Ethernet LIN FlexRay";

std::cout << findLongestWord(text) << '\n';
```

## 出力例

```text
Ethernet
```

## 追加確認

次のケースでも動作を確認してください。

```text
"ROS2 CAN ECU"       -> "ROS2"
"car software ECU"   -> "software"
"CAN LIN"            -> "CAN"
""                   -> ""
```

## ポイント

`std::string_view` は文字列そのものを所有せず、
元の文字列の一部分を参照することができます。

今回は、

1. スペースの位置を探す
2. `substr()` で単語を `std::string_view` として取得する
3. 最長の単語を記録する

という流れで実装してみてください。