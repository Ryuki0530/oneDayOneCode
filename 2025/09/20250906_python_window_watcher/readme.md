# OneDayOneCode：PyQt6 × Windows ウィンドウ監視ツール

## 概要
**PyQt6** で GUI アプリを作成し、**20 秒おき**に **Windows 上のトップレベル・ウィンドウ**（可視）を列挙して表に表示する。  
列挙は **Win32 API（ctypes）** を用いて実装し、**外部ライブラリ不使用**を基本とする（発展要件で `psutil` などを使うのは可）。

---

## 環境
- OS：Windows 11
- エディタ：VS Code
- 言語：Python 3.10+  
- GUI：PyQt6（`pip install PyQt6`）

---

## 必須機能
1. **定期更新**
   - 20 秒間隔でウィンドウ一覧を再取得し、表（テーブル）に反映する。
   - UI スレッドをブロックしない（`QTimer` 推奨）。

2. **取得対象**
   - トップレベルで **可視** なウィンドウのみ対象（`IsWindow`・`IsWindowVisible`）。
   - 既定では **タイトルが空のウィンドウは除外**。

3. **表示カラム（最低限）**
   - `タイトル`（`GetWindowTextW`）
   - `PID`（`GetWindowThreadProcessId`）
   - `クラス名`（`GetClassNameW`）
   - `アクティブ?`（列挙時に `GetForegroundWindow()` と一致なら ✅）

4. **UI 挙動**
   - テーブルの **列ソート**（昇順／降順）対応。
   - **手動更新**ボタン（即時再取得）。
   - ステータスバーに **最終更新時刻**を表示。
   - **ダブルクリック**で対象ウィンドウをアクティベート試行（`SetForegroundWindow`）。失敗時はエラー通知。

5. **安全性**
   - Win32 呼び出し失敗時は例外でアプリが落ちないこと（空文字や `None` で埋める等）。

---

## 入出力仕様

### 入力（CLI 引数）
- `--interval N`：更新間隔（秒）。省略時 **20**。
- `--include-empty`：空タイトルも **表示対象に含める**。
- `--all-styles`：可視判定を無視し **すべて列挙**（デバッグ用）。

**起動例**
```powershell
python .\main.py --interval 10 --include-empty
```

### 出力（GUI とファイル）
- GUI：テーブル表示、列ソート、手動更新、最終更新時刻表示。

---

## 画面仕様（参考）
- メニューバー
  - **表示**：`空タイトルを含める`、`非表示ウィンドウを含める`（チェックで切替）
  - **更新**：`今すぐ再取得`（F5）
- ツールバー
  - `更新` ボタン、`フィルタ` テキストボックス（発展）
- ステータスバー
  - `最終更新: YYYY-MM-DD HH:MM:SS`、`件数: N`

---

## 判定基準（チェックリスト）
- [ ] アプリが起動し、テーブルが表示される。  
- [ ] **20 秒ごと**に一覧が更新される。  
- [ ] 手動更新（F5 or ボタン）で即時再取得できる。  
- [ ] 列ソートが可能。  
- [ ] ダブルクリックで対象ウィンドウをアクティベート試行し、失敗時は通知。  
- [ ] `--interval` が機能する。  

---

## 実装の要点
- **Win32 列挙**（標準ライブラリのみ）  
  - `ctypes` で `EnumWindows` を呼び出し、コールバックで `HWND` を受け取る。  
  - 併用 API：`IsWindow`、`IsWindowVisible`、`GetWindowTextLengthW`、`GetWindowTextW`、`GetClassNameW`、`GetWindowThreadProcessId`、`GetForegroundWindow`、`SetForegroundWindow`。
- **モデル／ビュー分離**  
  - `QAbstractTableModel` でデータを管理し、再取得時は **差し替え更新**（`beginResetModel`/`endResetModel`）でも可。
- **スレッド不要方針**  
  - 列挙処理は短時間想定のため **`QTimer`** で十分。処理が重い場合は `QRunnable`/`QThreadPool` で非同期化。

---

## 発展要件（1つ以上 実装推奨）
- **実行ファイルパス**列を追加（PID→`OpenProcess` + `QueryFullProcessImageNameW` / `Psapi`）。  
  フォールバックとして `psutil.Process(pid).exe()` を使用可能。
- **タイトル／クラス名の部分一致フィルタ**（テキスト入力で即時絞り込み）。
- **除外ルール**（特定クラス名やタイトルのウィンドウを表示しない設定ファイル）。

---

## ディレクトリ構成（例）
```
winwatch/
  main.py                 # 引数処理・アプリ起動・タイマー
  ui_main.py              # QMainWindow（テーブル・ボタン・メニュー）
  winenum.py              # Win32 列挙（ctypes 実装）
  models/
    window_model.py       # QAbstractTableModel 実装
  exports/                # エクスポート出力先
  README.md               # 使い方・注意点
```

---

## 提出物
- 実行可能なソース一式（上記構成を推奨）
- `README.md`（起動方法・操作説明・発展要件の対応内容を明記）
- （任意）サンプルの CSV/JSON 出力ファイル

---

## 動作確認のヒント
- Notepad、PowerShell、VS Code を複数開き、**手動更新**と**定期更新**で件数と内容の変化を確認する。
- 空タイトルを含める設定の ON/OFF で行数が変化することを確認。
- エクスポート後、件数やフィールドが GUI と一致することを確認。
