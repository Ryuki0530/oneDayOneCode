# 🐍 20250627 - Python課題：画像フォルダの整理スクリプト

## 📝 課題内容

指定されたディレクトリ内にある **画像ファイル（JPEG, PNG）** を読み取り、  
**撮影日（Exif情報）や作成日時に応じてフォルダ分けする** スクリプトを作成してください。

## 📂 処理の内容

- 対象のフォルダを指定（例: `./images`）
- フォルダ内の `.jpg`, `.jpeg`, `.png` をスキャン
- 各画像の **撮影日（Exif）** または **作成日時** を取得
- `YYYY-MM`（例: `2023-11`）の形式でフォルダを作成して分類
  - 例：`./sorted/2023-11/xxx.jpg` のように移動
- Exif 情報がない画像は `unknown/` フォルダに移動


## ✅ 使用ライブラリ（標準＋補助）

- `os`, `shutil`, `datetime`
- `Pillow` (`PIL.Image`) を使用（Exif読み取り）

```
pip install Pillow
```
💡 実行例
```
$ python sort_images.py ./images
```
→ ./images の画像を撮影月ごとに ./sorted/YYYY-MM に移動
🔧 要件
OS: Windows11 (ファイルパスの扱いは os.path 推奨)
input_dir → sorted/YYYY-MM/ へ移動

実行時、処理結果をログとして出力（何枚処理されたかなど）

## 🧪 発展課題（任意）
ログファイルに記録（例: log.txt）

GUI でフォルダを選択できるようにする（tkinter.filedialog）

コマンドライン引数に対応

## 🔐 注意
.png など一部画像にはExifが無い場合も多いので、例外処理を入れてください。

---
