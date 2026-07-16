# OneDayOneCode
## 2026-07-16
### Python - Directory Snapshot Diff

# 問題

ディレクトリの状態をスナップショットとして保存し、
2つのスナップショットを比較して変更内容を表示するプログラムを作成してください。

---

## 入力

プログラム内で次のような辞書を用意してください。

snapshot_old = {
    "main.py": 1200,
    "config.json": 512,
    "README.md": 850,
    "data.csv": 15000
}

snapshot_new = {
    "main.py": 1400,
    "README.md": 850,
    "image.png": 3000,
    "data.csv": 15000
}

値はファイルサイズ(Byte)を表します。

---

## 出力例

Added:
image.png

Removed:
config.json

Modified:
main.py (1200 -> 1400)

Unchanged:
README.md
data.csv

---

## 条件

・追加されたファイル
・削除されたファイル
・サイズが変更されたファイル
・変更がないファイル

をそれぞれ分類して表示してください。

ファイル名はアルファベット順に表示してください。

---

## 発展課題

os.walk() を利用して実際のディレクトリを走査し、

{相対パス: ファイルサイズ}

という辞書を自動生成できるようにしてください。

さらに JSON に保存し、

python snapshot.py save ./project snapshot.json
python snapshot.py diff old.json new.json

のようなCLIツールへ発展させてください。