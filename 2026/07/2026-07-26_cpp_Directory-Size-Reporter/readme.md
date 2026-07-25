# OneDayOneCode

## タイトル
DirectorySizeReporter

## 問題
指定したディレクトリ内に存在する通常ファイルの情報を表示するプログラムを作成してください。

### 条件
- `std::filesystem` を使用すること
- サブディレクトリは探索しない
- 通常ファイルのみ対象とする
- 各ファイルについて以下を表示する
  - ファイル名
  - サイズ(Byte)
- 最後に
  - ファイル数
  - 合計サイズ(Byte)
  を表示する

### 実行例

対象ディレクトリ
```
sample/
├── a.txt (100 Byte)
├── b.txt (250 Byte)
├── image.png (500 Byte)
└── sub/
```

出力例
```
a.txt : 100 Byte
b.txt : 250 Byte
image.png : 500 Byte

Files : 3
Total : 850 Byte
```

### 発展課題（余裕があれば）
- サイズ順にソートして表示する
- 拡張子ごとのファイル数を集計する