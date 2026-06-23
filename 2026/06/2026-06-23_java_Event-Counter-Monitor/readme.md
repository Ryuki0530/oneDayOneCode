# 2026-06-23
# Event Counter Monitor

## 問題

イベントログを受け取り、イベント種別ごとの発生回数を集計するプログラムを作成せよ。

入力データは以下の固定配列とする。

String[] events = {
    "INFO",
    "WARN",
    "INFO",
    "ERROR",
    "WARN",
    "INFO"
};

## 要件

1. 各イベントの発生回数を集計する。
2. 結果を以下の形式で表示する。

INFO : 3
WARN : 2
ERROR : 1

3. Map を利用して実装すること。

## 追加課題（余裕があれば）

発生回数が最も多いイベントを表示する。

出力例

Most Frequent Event : INFO