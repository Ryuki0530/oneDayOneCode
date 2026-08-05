# 今日の課題: std::string_view を使った文字列解析

## 問題

1行のログ文字列を解析する関数を作成してください。

入力例

INFO:Robot started
ERROR:Motor failed
WARN:Low battery

### 要件

- `std::string_view` を引数に取る関数を作る
- `:` の位置を探す
- 左側を「ログレベル」
- 右側を「メッセージ」
- それぞれ表示する

### 実行例

入力:
ERROR:Motor failed

出力:
Level   : ERROR
Message : Motor failed

## 追加課題（余裕があれば）

ログレベルが
- INFO
- WARN
- ERROR

の3種類以外なら "UNKNOWN" と表示する。