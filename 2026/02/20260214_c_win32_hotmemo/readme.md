# 今日の課題（C / Win32 API）: 「ホットキー常駐メモ」(Tray + Global Hotkey)

## 概要
Windows の通知領域（タスクトレイ）に常駐する小さなユーティリティを C で作ります。  
グローバルホットキー（例: `Ctrl+Alt+M`）を押すと、**ミニ入力ウィンドウ**が出現し、入力したテキストを **追記ログ**として保存します。  
保存したログはトレイメニューから開けるようにします。

「Win32 API の基本部品（メッセージループ / ウィンドウ / トレイ / ホットキー / ファイルI/O）」を一通り触れる、実用寄りの題材です。

---

## 要求仕様

### 1) 常駐（トレイアイコン）
- 起動するとコンソールではなく **GUIアプリ**として動作（ウィンドウは通常表示しない）
- 通知領域にアイコンを出す（`Shell_NotifyIcon`）
- 右クリックでメニュー:
  - `Open Log` : ログファイルを既定アプリで開く（`ShellExecute`）
  - `Add Note...` : 入力ウィンドウを出す
  - `Exit` : 終了

### 2) グローバルホットキー
- `Ctrl + Alt + M` を登録（`RegisterHotKey`）
- 押されたら入力ウィンドウを表示してフォーカスを当てる

### 3) 入力ウィンドウ（ミニ）
- 小さいウィンドウ（例: 幅 420 / 高さ 120）
- 中身は
  - 1行入力 `EDIT`（複数行でもOK。余裕があれば複数行対応）
  - `Save` ボタン
  - `Cancel` ボタン or `Esc` で閉じる
- `Save` でログファイルに追記（タイムスタンプ付き）
  - 例: `[2026-02-14 21:03:12] ここに本文`

### 4) ログ保存
- 保存先は `"%APPDATA%\\HotMemo\\memo.log"` のようにする
  - `SHGetFolderPath` か `Known Folder` 系（MinGWなら `SHGetFolderPathA` が楽）
  - ディレクトリが無ければ作る（`CreateDirectory`）
- ファイル追記は Win32 の `CreateFile` + `WriteFile` を使う（C標準I/Oでも可だが、今回は Win32 寄りで）

---

## 入出力・挙動の例
- 起動 → トレイに常駐
- `Ctrl+Alt+M` → 入力ウィンドウが前面に出る
- テキスト入力 → Save → ログに追記 → ウィンドウは閉じる（または隠す）
- トレイメニュー `Open Log` → メモ帳などで開く

---

## 実装の制約（いい感じに “Win32らしく”）
- メッセージループは `GetMessage / TranslateMessage / DispatchMessage`
- トレイ通知は `WM_APP + n` などの独自メッセージで受ける
- リソース（.rc）無しでもOK（`LoadIcon(NULL, IDI_INFORMATION)` みたいな既定アイコンでもよい）
- 文字コードはまず ANSI (`A` 系 API) でOK（余裕があれば Unicode 対応してもいい）

---

## ビルド想定（MinGW-w64 / g++ でもOK）
※あなたの環境は `g++` が見えてるので、Cでもリンクは問題なしです。

- Cとしてビルドするなら:
  - `gcc main.c -municode -mwindows -luser32 -lshell32 -lgdi32 -lole32 -luuid -lcomdlg32`
- g++でビルドするなら:
  - `g++ main.c -mwindows -luser32 -lshell32 -lgdi32`

（実際に必要な lib は実装次第。トレイ + ウィンドウ + ShellExecute なら `-lshell32 -luser32` はだいたい要る）

---

## ヒント：Win32 API の要点（この課題で触るやつ）

### A) トレイアイコン（Shell_NotifyIcon）
- 構造体: `NOTIFYICONDATA`
- 追加: `Shell_NotifyIcon(NIM_ADD, &nid)`
- クリック通知:
  - `nid.uCallbackMessage = WM_APP + 1;`
  - そのメッセージの `lParam` に `WM_RBUTTONUP` / `WM_LBUTTONUP` などが入る

### B) 右クリックメニュー（TrackPopupMenu）
- `CreatePopupMenu` → `AppendMenu` → `TrackPopupMenu`
- 返るコマンドIDを `WM_COMMAND` と同様に処理してもOK

### C) グローバルホットキー（RegisterHotKey）
- `RegisterHotKey(hwnd, HOTKEY_ID, MOD_CONTROL|MOD_ALT, 'M');`
- 受信: `WM_HOTKEY`（`wParam` が ID）

### D) ログファイル追記（CreateFile/WriteFile）
- `CreateFile(path, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)`
- `WriteFile(h, buf, len, &written, NULL)`
- 改行は `\r\n` を推奨（Windowsなので）

### E) タイムスタンプ
- `GetLocalTime(&st)` で `SYSTEMTIME` を取って `snprintf` で整形

---

## 追加チャレンジ（余裕があれば）
1. `Ctrl+Alt+M` を押すたびに、ウィンドウ位置をマウス付近に出す（`GetCursorPos`）
2. 多重起動禁止（Mutex: `CreateMutex` + `GetLastError()==ERROR_ALREADY_EXISTS`）
3. `Save` したら軽いトースト風バルーン通知（`NIM_MODIFY` + `NIF_INFO`）
4. `Open Log` だけでなく `Open Folder` も追加（`ShellExecute`でフォルダパス）

---

## 提出物（あなたの oneDayOneCode 流儀に合わせる）
- `main.c` 1ファイルでOK（まずは動く最小構成）
- 余裕があれば `tray.c/h`, `ui.c/h`, `log.c/h` に分割して「責務分離」

---

## 注意（地雷）
- トレイの右クリックメニューは、`SetForegroundWindow(hwnd)` を呼ばないとメニューがすぐ消えることがある
- 終了時は `Shell_NotifyIcon(NIM_DELETE, &nid)` を忘れるとアイコンが幽霊化することがある
- `-mwindows` を付けるとコンソールが出ない（ログ出力は `OutputDebugString` が便利）

---