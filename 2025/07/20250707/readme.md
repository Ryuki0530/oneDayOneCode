# 問題名：学生の成績管理と並べ替え

### 問題内容：
あなたはあるクラスの成績データを管理するプログラムを作成することになりました。  
入力ファイル `students.txt` には、各行に以下の形式で学生情報が書かれています。

`<学生ID> <名前> <点数>`

例：
```
2023001 Tanaka 82
2023002 Suzuki 74
2023003 Yamada 91
```

このファイルを読み込んで、点数の高い順に並べ替え、以下の形式で `sorted_students.txt` に出力してください。

`<順位> <学生ID> <名前> <点数>`

例：
```
1 2023003 Yamada 91
2 2023001 Tanaka 82
3 2023002 Suzuki 74
```

### 入出力ファイル：
- 入力: `students.txt`（UTF-8, 改行コードLF）
- 出力: `sorted_students.txt`（点数の降順）

### 実装要件：
- 構造体を用いて学生情報を保持すること。
- ファイル読み込み・書き込みを行うこと。
- 任意のソートアルゴリズムを使って点数順に並べること。
- 最大100名までの学生に対応すること。

### 発展課題（余裕があれば）：
- 同点の場合は名前の辞書順に並べるよう改良してみよう。
- 標準入力から並び替え基準（点数 or 名前）を選べるようにしよう。