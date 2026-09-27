# Today's Python Challenge: Detect Sudden Load Changes

SO-101の関節から取得した負荷データを使い、急な負荷変化を検出してください。

## 入力データ

```python
loads = [12, 13, 12, 14, 15, 29, 31, 30, 16, 15]
threshold = 10
```

## 課題

隣り合う値の差の絶対値が `threshold` 以上になった箇所を探してください。各箇所について、変化後のインデックスと変化量を表示します。

期待する出力：

```text
index=5, change=14
index=8, change=14
```

`detect_changes(loads, threshold)` 関数を作り、検出結果を `(インデックス, 変化量)` のタプルのリストとして返してください。表示は関数の外で行ってください。

目安：10〜15分。標準ライブラリのみで解けます。