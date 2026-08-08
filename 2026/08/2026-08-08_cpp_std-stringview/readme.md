# 今日のC++課題：std::string_viewで文字列を切り出す

## 目的

`std::string_view` を使い、
文字列をコピーせずに部分文字列を扱ってみる。

## 問題

次のようなログ文字列があります。

~~~text
INFO:Robot dstarted
ERROR:Motor timeout
WARN:Battery low
~~~

以下の関数を実装してください。

~~~cpp
void printLog(std::string_view log);
~~~

## 仕様

1. `:` の位置を検索する
2. `:` より前をログレベルとして取得する
3. `:` より後をメッセージとして取得する
4. 次の形式で表示する

~~~text
Level   : ERROR
Message : Motor timeout
~~~

`:` が存在しない場合は、

~~~text
Invalid log
~~~

と表示してください。

## 条件

- C++17を使用する
- `std::string_view` を使用する
- 部分文字列の取得には `substr()` を使用する
- `std::string` への変換は禁止

## 実行例

~~~cpp
int main()
{
    printLog("INFO:Robot started");
    printLog("ERROR:Motor timeout");
    printLog("WARN:Battery low");
}
~~~

期待する出力：

~~~text
Level   : INFO
Message : Robot started

Level   : ERROR
Message : Motor timeout

Level   : WARN
Message : Battery low
~~~

## コンパイル

~~~bash
g++ -std=c++17 -Wall -Wextra main.cpp -o main
~~~