
# LogStreamAnalyzer

## 概要
センサやサーバーから逐次送信される数値ログを解析し、
直近のデータから異常値を検出するクラスを実装してください。

## 要件

`LogStreamAnalyzer` クラスを作成し、以下の機能を実装してください。

### 1. コンストラクタ
`__init__(self, window_size: int, threshold: float)`

- window_size: 保持する過去データの最大件数
- threshold: 異常と判定する偏差の閾値
- 過去データの保持には `collections.deque` を使用する
- window_size は1以上、threshold は0以上とする

### 2. データ追加
`add(self, value: float) -> bool`

以下の手順で処理してください。

1. 過去データが window_size 件未満の場合、異常判定せずFalseを返す
2. 過去データが window_size 件ある場合、その平均値を計算する
3. 新しい値と平均値の絶対差が threshold を超えた場合、異常と判定する
4. 新しい値を履歴に追加する
5. 異常判定結果をboolで返す

※ 異常値も履歴に追加してください。
※ 判定には新しい値を含めないでください。

### 3. 現在の平均値
`get_average(self) -> float`

- 保持しているデータの平均値を返す
- データが存在しない場合は0.0を返す

## 実行例

```python
analyzer = LogStreamAnalyzer(window_size=3, threshold=10)

print(analyzer.add(10))  # False
print(analyzer.add(12))  # False
print(analyzer.add(11))  # False
print(analyzer.add(50))  # True
print(analyzer.add(13))  # False
print(analyzer.get_average())  # 約24.67
```

## 制約
- 外部ライブラリは使用しない
- dequeのmaxlenを活用する
- データ追加時に履歴全体を走査せず、平均値を計算できるよう工夫する

## 発展課題（任意）
- 直近N件の標準偏差を計算するメソッドを追加する
- 異常検出回数を記録する
- thresholdを固定値ではなく標準偏差の倍率で指定できるようにする
