# Seat Reservation

使用言語: Java
目安: 15〜20分

## 課題
座席番号1〜10の予約を管理するプログラムを作成してください。
boolean配列の代わりに、標準ライブラリのBitSetを使います。

## 実装要件
1. 次の命令を処理してください。
   - BOOK n: 座席nを予約する。
   - CANCEL n: 座席nの予約を取り消す。
   - NEXT: 空席のうち、最小の座席番号を表示する。

2. 命令ごとに、次の結果を表示してください。
   - 予約成功: BOOKED
   - 予約済みの座席を予約: ALREADY BOOKED
   - 取消成功: CANCELED
   - 未予約の座席を取消: NOT BOOKED
   - NEXT: 座席番号。満席ならFULL

3. 座席番号が1〜10の範囲外ならINVALIDと表示し、
   予約状態は変更しないでください。

4. 命令はコード内のString配列に用意して構いません。
   命令の形式は正しいものとします。

## 入力例
BOOK 1
BOOK 2
BOOK 1
NEXT
CANCEL 1
NEXT
CANCEL 1
BOOK 11

## 出力例
BOOKED
BOOKED
ALREADY BOOKED
3
CANCELED
1
NOT BOOKED
INVALID

## ヒント
- import java.util.BitSet;
- get(i): ビットiが立っているか確認
- set(i): ビットiを立てる
- clear(i): ビットiを消す
- nextClearBit(i): i以降で最初の未設定ビットを探す

座席番号とビットの添字をどう対応させるか決めてください。
BitSetは指定したサイズを超えて拡張できるため、
座席番号の範囲チェックは別途必要です。

## 完了条件
入力例と同じ結果が出ることに加え、
全席予約後のNEXTでFULL、
その後に座席5を取り消すとNEXTで5が出れば完成です。