# 今日の課題: Java CANログしきい値チェッカー

## 内容
CANログとして、車速データが時系列で与えられる。
各行には CAN ID と 速度[km/h] が与えられる。

CAN ID が 100 のデータだけを車速として扱い、
速度が 60km/h 以上なら WARN、
60km/h 未満なら INFO として出力しなさい。

CAN ID が 100 以外の行は無視する。

## 入力
N
id_1 speed_1
id_2 speed_2
...
id_N speed_N

## 制約
1 <= N <= 100
id は整数
speed は整数

## 出力
CAN ID が 100 の行について、以下の形式で出力する。

INFO speed
または
WARN speed

## 入力例
6
100 45
101 1
100 60
102 30
100 80
100 59

## 出力例
INFO 45
WARN 60
WARN 80
INFO 59

余裕があれば追加仕様として、最後に WARN の回数も出してください。
追加出力例

WARN_COUNT 2