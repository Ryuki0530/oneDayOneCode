# 課題: WinAPI トレイ常駐「NOSLEEP」—スリープ阻止ツール

## 概要
Windows API **のみ**で実装する、通知領域（トレイ）常駐の軽量ユーティリティを作成する。左クリックでオン/オフをトグル、右クリックでメニュー操作ができ、オンの間は PC のスリープ・ディスプレイ消灯を抑止する。

## 目標
- Win32のメッセージ駆動・隠しウィンドウ・通知領域アイコン・コンテキストメニュー・電源管理APIの基本を網羅して実装する。
- MinGW-w64 の GCC でそのままビルドできる **単一Cファイル** を完成させる。

## 必須要件
1. **起動時**：ウィンドウは表示せず、通知領域に常駐（アイコンとツールチップを表示）
2. **左クリック**：スリープ抑止の **有効/無効をトグル**
3. **右クリック**：コンテキストメニュー（「有効/無効」「終了」）
4. **有効時**：`SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_AWAYMODE_REQUIRED)` を呼び、スリープ/画面オフを抑止  
   **無効時**：`SetThreadExecutionState(ES_CONTINUOUS)` に戻す
5. **状態変化時**：ツールチップ更新＋バルーン通知（`NIF_INFO`）でユーザーに伝える
6. **終了時**：アイコンを確実に削除し、抑止状態を解除して終了
7. **Unicode対応**：エントリポイントは `wWinMain`、ワイド版 API を使用

> 注記：`ES_AWAYMODE_REQUIRED` は Vista 以降で有効。未対応環境では無視されても可とする。

## 実装詳細（指針）
- **隠しウィンドウ**：`RegisterClassW` → `CreateWindowExW`（非表示）。メッセージループで常駐
- **通知領域アイコン**：`NOTIFYICONDATAW` + `Shell_NotifyIconW(NIM_ADD/NIM_MODIFY/NIM_DELETE)`
  - コールバックメッセージ用に `WM_APP + 1` などを割り当て
  - 左クリック／右クリックの判定（`WM_LBUTTONUP` / `WM_RBUTTONUP`）
- **コンテキストメニュー**：`CreatePopupMenu` → `AppendMenuW` → `TrackPopupMenu`  
  （`SetForegroundWindow` を呼んでメニューが閉じなくなる問題を回避）
- **電源管理**：状態トグル関数を用意して `SetThreadExecutionState` を一元管理
- **バルーン通知**：`NOTIFYICONDATAW.szInfo` / `szInfoTitle` / `dwInfoFlags` を設定し `NIM_MODIFY`

## ビルド方法（例：x86_64）
```bash
gcc main.c -municode -Wall -Wextra -O2 -o nosleep.exe -luser32 -lshell32 -lgdi32
```

## 確認手順
1. 実行するとトレイに「NOSLEEP: 無効」のツールチップでアイコンが出る
2. 左クリックで「有効」に切り替わり、バルーン通知「抑止中」が表示される
3. 右クリックメニューからも切り替え・終了ができる
4. 終了後、トレイにアイコンが残存しないこと

## 受け入れ基準
- クリック操作でトグルでき、**有効時は一定時間操作しなくてもディスプレイが消灯しない**（OS設定依存の猶予はある）
- 通知（バルーン/ツールチップ）が状態と一致
- プロセス終了時に `SetThreadExecutionState(ES_CONTINUOUS)` を呼び戻している
- 例外やクラッシュなく常駐・終了できる

## 参考API（キーワード）
- `RegisterClassW`, `CreateWindowExW`, `DefWindowProcW`, `GetMessageW`, `DispatchMessageW`
- `NOTIFYICONDATAW`, `Shell_NotifyIconW`, `LoadIcon`, `LoadCursor`
- `CreatePopupMenu`, `AppendMenuW`, `TrackPopupMenu`, `SetForegroundWindow`
- `SetThreadExecutionState`, フラグ：`ES_CONTINUOUS`, `ES_SYSTEM_REQUIRED`, `ES_DISPLAY_REQUIRED`, `ES_AWAYMODE_REQUIRED`

## 提出物
- `main.c`（単一ファイル、ビルド手順付きコメント推奨）

## 発展課題（任意）
- **グローバルホットキー**：`RegisterHotKey` で Ctrl+Shift+N などをトグルに割当
- **アイコン切替**：有効/無効で `LoadImage` や埋め込みリソースを使って差し替え
- **自動オフ**：`SetTimer` で N 分後に抑止解除
- **起動オプション**：`--enable` 指定で起動直後から抑止を有効化
