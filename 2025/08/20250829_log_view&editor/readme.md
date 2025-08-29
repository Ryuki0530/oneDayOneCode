# JFrameでつくる「ログビューア & フィルタエクスポーター」

**今日の言語：Java（Swing / JFrame）**  
Windows 11 + VSCode を想定。標準入力/出力だけでなく、**ファイル入出力**・**GUI 操作**・**コマンドライン引数**の3本立てで練習します。

---

## 概要

テキストログ（UTF-8 の `.log`/`.txt`/`.csv` 等）を読み込み、GUI で閲覧・検索・フィルタし、結果を**保存**または**標準出力にエクスポート**するデスクトップアプリを作成してください。

- ベースは `JFrame`。  
- メニューやツールバーから**ファイルを開く**／**上書き保存**／**名前を付けて保存**／**エクスポート（stdout/ファイル）**。  
- テキストの表示は自由（`JTextArea` でも `JTable` でも可）。  
- **ドラッグ＆ドロップ**でファイルをウィンドウへ投げ込んだら開けること。  
- 大きめファイルも固まらないよう、**SwingWorker** 等で非同期読み込み。  
- **正規表現フィルタ**で行単位に絞り込み。ヒット件数はステータスバーに表示。  
- コマンドライン引数で起動時に自動オープン／フィルタ適用ができること。

---

## 必須仕様

### 1) 起動オプション（コマンドライン引数）
- `--open <path>` … アプリ起動時にファイルを開く  
- `--filter "<regex>"` … 起動時に正規表現フィルタを適用  
- `--export stdout|<path>` … 現在のフィルタ結果を標準出力またはファイルへエクスポートして**アプリを終了**  
- **例**
  - `java Main --open ".\logs\app.log" --filter "ERROR|WARN"`
  - `java Main --open ".\logs\app.log" --filter "userId=\d+" --export stdout`

> 解析は**順不同**で良いですが、矛盾した指定（例: `--export` あるのにファイル未読）には丁寧にエラーメッセージを標準エラー出力へ。

### 2) GUI（JFrame 構成）
- **メニューバー**
  - ファイル: 開く / 上書き保存 / 名前を付けて保存 / エクスポート / 終了
  - 表示: 行番号の表示切替（任意実装）/ 折り返し切替
  - ヘルプ: バージョン情報（ダイアログ）
- **ツールバー**（ボタン）
  - 開く、保存、エクスポート、検索欄（`JTextField`）、「正規表現」チェック、適用ボタン
- **中央ビュー**
  - テキスト全体 or フィルタ結果をスクロール表示（`JScrollPane`）
- **ステータスバー**
  - 読み込み状態（準備中/完了/件数）、カーソル位置（任意）、メモリ使用量（任意）

### 3) ファイル入出力
- 読み込みは **UTF-8** 前提。  
- 保存は現在表示中テキスト（フィルタの有無は任意選択可。要 UI 表示）。  
- **Drag & Drop**:
  - ウィンドウへファイルをドロップ → 即読み込み。
  - `TransferHandler` での実装を推奨。

### 4) フィルタ機能（正規表現）
- テキストを**行単位**で評価。  
- 正規表現モードON時は `Pattern` を使い、OFF時は部分一致。  
- フィルタ結果の**件数**をステータスバーに表示。  
- フィルタは**非同期で再計算**し、UI が固まらないこと。

### 5) エクスポート
- メニュー/ツールバーから実行可能。
- 出力先が `stdout` の場合は、**標準出力へ行単位で書き出し**、GUI 起動時（インタラクティブ）なら完了ダイアログ表示。  
- ファイルパス指定時は UTF-8 で書き出し、完了後にステータス更新。

---

## 入出力フォーマット

### 入力（ファイル）
- UTF-8 テキスト。1 行が 1 レコード。
- 行末は `\n`/`\r\n` どちらでもよい。

### 出力（標準出力）
- **エクスポート時のみ**、現在のフィルタ結果を**そのまま**出力。
- 末尾改行は各行に維持。

### エラー（標準エラー出力）
- ファイルが見つからない／読み込み不可／正規表現が不正 などは、人間が読める短い1行メッセージ。

---

## 実行例

```
# 1) ふつうに起動してGUI操作
javac Main.java
java Main

# 2) 起動と同時にファイルを開き、正規表現で抽出（GUI表示）
java Main --open ".\sample.log" --filter "ERROR|WARN"

# 3) 非表示でエクスポート完了 → 終了（CI用途）
java Main --open ".\sample.log" --filter "userId=\d+" --export stdout > filtered.txt
```

ディレクトリ構成（提案）

```
2025-08-29/
  ├─ Main.java
  ├─ view/
  │   ├─ LogFrame.java        // JFrame本体
  │   ├─ StatusBar.java       // ステータスバー
  │   └─ ToolbarPanel.java    // ツールバー
  ├─ model/
  │   ├─ LogDocument.java     // 生テキスト保持 & フィルタ適用
  │   └─ FilterOptions.java   // 正規表現ON/OFF、パターン文字列など
  ├─ io/
  │   ├─ FileLoader.java      // SwingWorkerで非同期読み込み
  │   └─ Exporter.java        // stdout / file への出力
  └─ sample.log
```
※ 単一ファイル提出でも可。構成は自由。

## 評価観点
JFrame 活用（メニュー、ツールバー、中心ビュー、ダイアログ）

非同期処理（読み込み・フィルタで UI がフリーズしない）

入出力の網羅（ファイル I/O・stdout への出力・引数解析）

エラーハンドリング（不正正規表現、読み込み失敗時の扱い）

ユーザビリティ（件数表示、状態表示、ショートカットなど）

## ヒント
非同期: SwingWorker<Void, List<String>> などで行配列を読み込み、done() でモデルに反映。

DnD: setTransferHandler(new TransferHandler() { importData(...) }) で javaFileListFlavor を受け取る。

巨大ファイル: 行ごとの遅延読み込み→モデルに溜める、フィルタは別スレッドで計算して差し替え。

検索高速化: 正規表現は事前に Pattern をコンパイル。単純包含は String.contains。

UI更新は Event Dispatch Thread（EDT）で。SwingUtilities.invokeLater(...) を活用。

## 追加チャレンジ（任意）
Ctrl+F で検索ダイアログ、F3 で次を検索。

行番号表示（JTextArea の RowHeader 利用など）。

文字コード自動判定（BOM/ヒューリスティック）。

フィルタ式の履歴（コンボボックス化）。

ハイライト表示（ヒット箇所に Highlighter）。

ビルド & 実行コマンド（Windows / VSCode）

# コンパイル
javac -encoding UTF-8 .\Main.java

# GUI 起動
java Main

# 起動時オプション例
java Main --open ".\sample.log" --filter "ERROR|WARN"
java Main --open ".\sample.log" --filter "userId=\d+" --export stdout > filtered.txt