# RangeSumQuery

使用言語: C++17
所要時間の目安: 20〜30分

## 目的
累積和を使い、整数配列の指定区間の合計を効率よく求めてください。

## 実装要件
1. 次のクラスを実装してください。
   - RangeSumQuery(const std::vector<int>& values)
     配列を受け取り、累積和を作成します。
   - std::optional<long long> query(int left, int right) const
     インデックスleftからrightまで（両端を含む）の合計を返します。

2. インデックスは0始まりです。
   left < 0、left > right、rightが配列の範囲外の場合は
   std::nulloptを返してください。空配列への問い合わせも同様です。

3. 合計はlong longで扱ってください。
   構築はO(N)、各問い合わせはO(1)にしてください。
   問い合わせごとに区間をループしてはいけません。

4. main関数で結果を表示してください。
   std::nulloptの場合はINVALIDと表示します。
   配列や問い合わせはコード内に直接記述して構いません。

## 入力例
配列: [3, -2, 5, 1, -4, 6]

問い合わせ:
query(0, 2)
query(2, 5)
query(3, 3)
query(0, 5)
query(-1, 2)
query(4, 2)
query(0, 6)

## 期待出力
6
8
1
9
INVALID
INVALID
INVALID

## 完了条件
- 入力例の結果が一致すること。
- 空配列へのquery(0, 0)がINVALIDになること。
- [2000000000, 2000000000]の全区間の合計が
  4000000000になること。