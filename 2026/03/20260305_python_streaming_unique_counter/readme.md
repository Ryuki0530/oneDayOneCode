
# Streaming Unique Counter

イベントログがストリームとして与えられる。

各行は次の形式である。

timestamp user event

例:
1 alice login
2 bob click
3 alice click

同じユーザーが同じイベントを **W秒以内** に再度発生させた場合、
それは **重複イベントとして無視する**。

重複でないイベントだけを集計する。

## 入力

1行目
W

2行目以降
timestamp user event

timestamp は昇順で与えられる。

## 出力

有効イベントのみをカウントし、以下を出力する。

TOTAL <総イベント数>

EVENT <event名> <回数>  
（回数降順、同数なら辞書順）

USER <user名> <回数>  
（回数降順、同数なら辞書順）

## 入力例

5
1 alice login
2 alice login
4 bob click
7 alice login
8 bob click
10 bob click

## 出力例

TOTAL 4
EVENT click 2
EVENT login 2
USER alice 2
USER bob 2
