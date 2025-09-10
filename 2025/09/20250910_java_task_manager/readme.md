# OneDayOneCode: Java課題（タスク管理に特化した最小GUI）

目的：**標準ライブラリのみ（Swing/AWT/`ProcessBuilder`/`java.nio`）**で、Windowsの `tasklist` / `taskkill` を呼び出す**シンプルなタスクマネージャ**を作る。  
余計な機能は足さず、**閲覧・フィルタ・終了**の3点に**厳しく絞る**。

---

## 課題名
**WinTaskLite — 極小タスクマネージャ**

## 必須要件
1. **GUI（Swing）**
   - 上部：検索ボックス（プレースホルダ「名前 or PID を入力」）
   - 中央：**プロセス一覧 `JTable`**
     - 取得は `ProcessBuilder("cmd", "/c", "tasklist /fo csv /nh")`
     - 列：`Image Name` / `PID` / `Mem Usage`
   - 下部ボタン：
     - `更新`：一覧を再取得して反映
     - `選択を終了`：選択行の PID へ `taskkill /PID <pid> /F` を実行（確認ダイアログあり）

2. **フィルタ**
   - 検索ボックスへの入力で**部分一致フィルタ**（`Image Name` または `PID`）
   - `TableRowSorter` を使用し、**昇降順ソート**に対応（少なくとも `Mem Usage` で数値ソート）

3. **非ブロッキング**
   - `tasklist` 取得・`taskkill` 実行は EDT（イベントディスパッチスレッド）を塞がないよう、`ExecutorService` または `SwingWorker` で実行
   - 失敗時は `JOptionPane` でエラー表示（アクセス拒否・プロセス終了済み等）

4. **CSV パース**
   - 標準ライブラリのみで**簡易CSVパーサ**を実装（ダブルクォートとカンマを考慮）

## 入出力仕様
- **入力**：GUI操作（検索テキスト、テーブル選択、ボタン）
- **出力**：プロセス一覧の表示、終了結果のダイアログ表示、標準エラーへの例外ログ（任意）

## 実装ヒント
- `tasklist` 出力例（CSV, ヘッダなし）  
  `Image Name","PID","Session Name","Session#","Mem Usage"`
- `Mem Usage` は `"12,345 K"` のような文字列。カンマと ` K` を除去して数値化（int / long）→ソート用に保持
- 文字コードは通常 CP932。最初はデフォルトで読み、化ける場合のみ `Charset.forName("MS932")` を試す程度で可
- UI 反映は `SwingUtilities.invokeLater(...)`

## ディレクトリ構成（例）
```
Main.java
ui/MainFrame.java
ui/ProcessTableModel.java
core/CmdRunner.java
core/CsvUtil.java
core/ProcessFetcher.java
out/
```

## 受け入れ基準（チェック）

- 1000件程度のプロセスでも UI が固まらない（取得・killがEDT外）
- 名前/ PID による部分一致フィルタが効く
- Mem Usage で昇降順ソートができる（数値として）
- 選択を終了 実行前に確認ダイアログが出る／結果の例外処理がある
- 例外時にユーザに分かるエラーダイアログが出る

## 任意の小拡張（追加しすぎないこと）

- Ctrl+F で検索ボックスにフォーカス
- Enter で即フィルタ適用、Esc でクリア
- PID ダブルクリックでクリップボードにコピー

## ねらい
Unix哲学にならい、単機能を小さく、正しく、速く。
次の段階で必要になった最小限だけを足せる設計に留めること。