# TimeSeriesRangeQuery

## 概要

タイムスタンプ付きのセンサーデータを管理し、
指定された時間範囲内のデータを効率的に取得する
TimeSeriesRangeQueryクラスを実装してください。

データの検索には、線形探索ではなく二分探索を使用してください。

## 実装仕様

以下のメソッドを実装してください。

### 1. コンストラクタ

```python
def __init__(self):
```

時系列データを管理するための内部データ構造を初期化する。

### 2. データの追加

```python
def add(self, timestamp: int, value: float) -> None:
```

タイムスタンプと測定値を登録する。

- データは時系列順に管理すること。
- タイムスタンプの登録順序は保証されない。
- 同一タイムスタンプのデータが存在する場合は、
  既存の測定値を上書きすること。

### 3. 範囲検索

```python
def query(self, start: int, end: int) -> list:
```

指定された時間範囲内のデータを取得する。

- start <= timestamp <= end を満たすデータを返す。
- 戻り値は(timestamp, value)のタプルのリストとする。
- タイムスタンプの昇順で返すこと。
- 該当するデータが存在しない場合は空リストを返す。
- start > endの場合はValueErrorを送出する。

### 4. 直近データの取得

```python
def latest(self, timestamp: int) -> tuple | None:
```

指定されたタイムスタンプ以下のデータのうち、
最も新しいデータを取得する。

該当するデータが存在しない場合はNoneを返す。

## 動作例

```python
def main():
    series = TimeSeriesRangeQuery()

    series.add(100, 12.5)
    series.add(300, 18.0)
    series.add(200, 15.2)
    series.add(500, 20.1)
    series.add(400, 17.8)

    print(series.query(200, 400))
    print(series.query(150, 350))
    print(series.query(600, 800))

    print(series.latest(350))
    print(series.latest(100))
    print(series.latest(50))

    series.add(300, 99.9)

    print(series.query(300, 300))


if __name__ == "__main__":
    main()
```

## 期待される出力

```text
[(200, 15.2), (300, 18.0), (400, 17.8)]
[(200, 15.2), (300, 18.0)]
[]
(300, 18.0)
(100, 12.5)
None
[(300, 99.9)]
```

## 制約

- Pythonの標準ライブラリのみ使用可能。
- bisectモジュールを使用すること。
- データ追加時にリスト全体をソートし直さないこと。
- queryの時間計算量はO(log N + K)とすること。
- latestの時間計算量はO(log N)とすること。
- addの時間計算量はO(N)以下とすること。

Nは登録済みデータ数、Kは検索結果の件数とする。

