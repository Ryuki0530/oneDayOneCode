
# SensorDataInterpolator

## 問題概要

複数のセンサーから送信される時系列データを管理し、
任意の時刻におけるセンサー値を取得するクラス
`SensorDataInterpolator` を実装してください。

センサーデータは必ずしも一定間隔で届くとは限りません。
そのため、データが存在しない時刻については、
前後のデータを利用して線形補間を行います。

## 要件

### 1. コンストラクタ

```python
class SensorDataInterpolator:
    def __init__(self):
        pass
```

複数のセンサーのデータを管理できるようにしてください。

### 2. データ登録

```python
def add_data(self, sensor_id: str, timestamp: float, value: float) -> None:
```

指定したセンサーのデータを登録します。

- センサーIDは文字列
- timestampは時刻を表す数値
- valueはセンサーの測定値
- データは時刻順に登録されるとは限らない
- 同一センサーの同一時刻にデータを登録した場合、最新の値で上書きする

### 3. データ取得

```python
def get_value(self, sensor_id: str, timestamp: float) -> float | None:
```

指定時刻のセンサー値を返します。

以下のルールに従ってください。

1. 指定時刻にデータが存在する場合、その値を返す
2. 指定時刻が2つのデータの間にある場合、線形補間した値を返す
3. 指定時刻が登録済みデータの範囲外の場合、Noneを返す
4. センサーIDが存在しない場合、Noneを返す

線形補間の計算式：

y = y0 + (y1 - y0) * (t - t0) / (t1 - t0)

t0, t1：前後のデータの時刻
y0, y1：前後のデータの値
t：取得したい時刻

### 4. データ範囲取得

```python
def get_range(self, sensor_id: str, start: float, end: float) -> list[tuple[float, float]]:
```

指定範囲内に存在する実測データを時刻順で返します。

- startとendは両端を含む
- 補間値は含めない
- データが存在しない場合は空リストを返す
- start > end の場合も空リストを返す

## 実行例

```python
interpolator = SensorDataInterpolator()

interpolator.add_data("motor1", 0.0, 10.0)
interpolator.add_data("motor1", 2.0, 30.0)
interpolator.add_data("motor1", 5.0, 60.0)

print(interpolator.get_value("motor1", 1.0))
# 20.0

print(interpolator.get_value("motor1", 3.0))
# 40.0

print(interpolator.get_value("motor1", 6.0))
# None

print(interpolator.get_range("motor1", 1.0, 5.0))
# [(2.0, 30.0), (5.0, 60.0)]

interpolator.add_data("motor1", 2.0, 50.0)

print(interpolator.get_value("motor1", 2.0))
# 50.0
```

## 制約

- Python 3.10以上
- 外部ライブラリは禁止
- 標準ライブラリは使用可能
- センサーごとに最大100,000件のデータを登録可能
- timestampとvalueは有限の数値とする

