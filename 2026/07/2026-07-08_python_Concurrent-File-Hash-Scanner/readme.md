# OneDayOneCode

## タイトル
Python Concurrent File Hash Scanner

## 問題
指定したディレクトリ以下の全ファイルについてSHA-256ハッシュを計算するプログラムを作成してください。

ただし、ハッシュ計算は複数スレッドを用いて並列に実行してください。

## 条件

- pathlib を使用する
- hashlib を使用する
- concurrent.futures.ThreadPoolExecutor を使用する
- サブディレクトリも探索する
- ファイルサイズも表示する

## 出力例

example.txt
  Size : 1248 bytes
  SHA256 : 0d4a1185...

image.png
  Size : 243871 bytes
  SHA256 : 2b6af0d4...

...

Processed 152 files.

## ボーナス課題

以下のどちらかを実装してください。

### Easy
ハッシュ値が同じファイル（重複ファイル）を最後に一覧表示する。

### Hard
処理時間を計測し、

- max_workers=1
- max_workers=4
- max_workers=8

で実行時間を比較する。

ThreadPoolExecutor がどの程度効果を発揮するか確認してください。