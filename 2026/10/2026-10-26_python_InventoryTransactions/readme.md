# Inventory Transactions

使用言語: Python
目安: 15〜20分

## 課題
商品の在庫と入出庫履歴を受け取り、在庫を更新する関数
apply_transactions(stock, transactions)
を実装してください。

辞書による状態管理と、更新前の条件チェックを練習します。

## 要件
1. stockは「商品名: 在庫数」の辞書です。
2. transactionsは「商品名, 増減数」のタプルのリストです。
   正数は入庫、負数は出庫を表します。
3. 履歴を先頭から処理してください。
   更新後の在庫が負になる取引は適用せず、
   その取引のインデックスを失敗リストに追加してください。
4. 未登録の商品は在庫0として扱います。
   取引が成功した場合のみ辞書に登録してください。
5. 更新後の辞書と失敗リストを返してください。
   元のstockは変更しないでください。

## 入力例
stock = {"apple": 5, "banana": 2}

transactions = [
    ("apple", -3),
    ("banana", -4),
    ("orange", 3),
    ("apple", -3),
    ("banana", 1),
    ("grape", -1),
]

## 呼び出し例
updated, failed = apply_transactions(stock, transactions)
print(updated)
print(failed)
print(stock)

## 期待出力
{'apple': 2, 'banana': 3, 'orange': 3}
[1, 3, 5]
{'apple': 5, 'banana': 2}

## 完了条件
- 入力例で期待出力を得られること。
- 在庫がちょうど0になる出庫は成功すること。
- 履歴が空の場合、元の辞書と同じ内容の新しい辞書と、
  空の失敗リストを返すこと。