# oneDayOneCode: Java課題（所要10–20分）

## 問題: 売上ログの集計

標準入力からCSV形式の売上ログを読み込み、**日別合計**と**カテゴリ別合計**を集計して出力してください。

### 入力
- 各行は `YYYY-MM-DD,category,amount` の形式  
  - `YYYY-MM-DD` は日付（例: `2025-10-17`）  
  - `category` は英数と`-_`のみ（例: `food`, `drink`, `set-menu`）  
  - `amount` は整数（正負どちらも可）
- 行数は最大 **200,000** 行
- 入力は **EOF** まで続く
- 形式が不正な行は **無視** してよい

### 出力
1. 見出し行 `# daily` を出力  
   続けて **日付昇順** に `YYYY-MM-DD,total` を1行ずつ出力
2. 見出し行 `# category` を出力  
   続けて **合計金額の降順**, 金額同点は **カテゴリ名昇順** に `category,total` を1行ずつ出力

#### 例
入力:
2025-10-16,food,1200
2025-10-16,drink,300
2025-10-17,food,800
2025-10-17,food,-200
2025-10-17,side,150
bad-line-here

出力:
daily
2025-10-16,1500
2025-10-17,750

category
food,1800
drink,300
side,150

### 制約・注意
- 時刻は登場しません（終日単位で集計）
- メモリ効率と速度を意識して実装してください（`HashMap` や `TreeMap` の活用）
- 入力は大きい可能性があるため、`BufferedReader` を使ってください

### 実装ヒント（任意）
- 行を `split(",", 3)` で分割  
- 日付の検証は最低限でOK（`YYYY-MM-DD` の形か簡易チェック）
- 日別は `TreeMap<String, Long>` にすると自然に昇順  
- カテゴリ別は `HashMap<String, Long>` → 最後に `List` に移してカスタムソート

### コンパイル & 実行例（Windows / VSCode ターミナル）
```bash
javac Main.java
type input.txt | java Main