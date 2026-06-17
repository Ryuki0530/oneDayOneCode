# Python課題：CANログ風データから車速異常を検出する

## 目的

CSV形式のCANログ風データを読み込み、車速データを解析して、急加速・急減速を検出するプログラムを作成する。

## 入力データ

以下のようなCSVファイル `can_log.csv` を用意する。

```csv
time,id,data
0.0,0x100,00
0.5,0x100,05
1.0,0x100,0A
1.5,0x100,14
2.0,0x100,28
2.5,0x100,2D
3.0,0x200,FF
3.5,0x100,10
```

## 仕様

- `id` が `0x100` の行だけを車速データとして扱う
- `data` は16進数文字列で、車速[km/h]を表す
  - 例：`0A` → 10 km/h
  - 例：`28` → 40 km/h
- 連続する車速データ同士を比較する
- 1秒あたりの速度変化量を計算する

## 異常判定

以下のどちらかを満たす場合、異常として出力する。

- 加速度が `+20 km/h/s` 以上
- 減速度が `-20 km/h/s` 以下

## 出力例

```text
[ALERT] time=2.0s speed=40km/h delta=32.0km/h/s
[ALERT] time=3.5s speed=16km/h delta=-58.0km/h/s
```

※ 実際の出力内容は、作成したCSVの値に応じて変わってよい。

## 実装条件

- Python標準ライブラリのみを使う
- CSV読み込みには `csv` モジュールを使う
- 関数を最低2つ作ること
  - 例：`load_can_log()`
  - 例：`detect_speed_alerts()`
- `if __name__ == "__main__":` を使うこと

## 発展課題

余裕があれば、以下にも対応する。

1. `0x100` 以外のIDを無視した件数を表示する
2. 最大速度、最小速度、平均速度を表示する
3. 異常判定のしきい値をコマンドライン引数で指定できるようにする

## 補足

CSV読み込みには `csv.DictReader` を使うとよい。

```python
import csv

with open("can_log.csv", newline="") as f:
    reader = csv.DictReader(f)
    for row in reader:
        print(row["time"], row["id"], row["data"])
```

16進数文字列を数値に変換するには、`int(文字列, 16)` を使う。

```python
speed = int("0A", 16)
print(speed)  # 10
```