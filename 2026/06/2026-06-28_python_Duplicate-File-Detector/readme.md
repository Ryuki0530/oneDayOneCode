# OneDayOneCode

# 2026-06-28
# Python
# Title: Duplicate File Detector

## 問題

指定したディレクトリ以下を再帰的に探索し、
ファイルサイズが同じファイルを一覧表示してください。

### 仕様

- pathlib を使用する
- 再帰探索を行う
- ファイルサイズ(Byte)ごとにグループ化する
- 同じサイズのファイルが2個以上ある場合のみ表示する
- 表示はサイズの昇順

### 実行例

$ python duplicate_file_detector.py .

=== 1024 Bytes ===
./a.txt
./dir1/b.txt

=== 4096 Bytes ===
./image1.png
./backup/image_copy.png

### 発展（時間があれば）

サイズだけでは誤判定があるため、
hashlib.sha256 を利用して
同じサイズのファイルのみハッシュ比較を行い、
「本当に同じ内容のファイル」だけ表示してください。

## 学習ポイント

- pathlib.Path
- Path.rglob()
- defaultdict
- 辞書へのリスト格納
- hashlib（発展）