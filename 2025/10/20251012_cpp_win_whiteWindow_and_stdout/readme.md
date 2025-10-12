
# oneDayOneCode 課題（2025-10-12）— C++ × Windows API

## 概要
今日は **2本立て**。  
1) まっさらのネイティブ Win32 ウィンドウを作る。  
2) コマンドライン引数を受け取り **標準出力に結果を出す**小ツールを作る。

すべて **C++17 以上**、**Windows API（Win32）** を使用。開発環境は **Windows 11 + VS Code** を想定。

---

## 課題A：無地のウィンドウ（Win32 GUI）
### 要件
- 800×600 のウィンドウを作成する（背景は単色、描画なし）。
- タイトルは `OD1C Window - 2025-10-12`。
- 画面中央に表示する。
- 閉じる操作（×/Alt+F4）で終了する。
- Unicode 対応（`UNICODE` と `_UNICODE` を有効化）。
- **WinMain** エントリで作成（`main` ではない）。

### 仕様チェック
- 起動後に **メッセージループ**が動作していること（CPU を食い尽くさない）。
- `WM_DESTROY` で `PostQuitMessage(0)` を呼ぶこと。

### 入出力
- 入力：なし
- 出力：なし（GUIのみ）

---

## 課題B：コマンドライン出力ツール（Console）
### 要件
- プログラム名：`od1c_echo`（任意でOK）
- **Unicode コマンドライン**を安全に取得する（`GetCommandLineW` + `CommandLineToArgvW`）。
- 書き出しは **標準出力**（`WriteConsoleW` ただしリダイレクト時は `WriteFile`）で行う。
- 下記のように動作：
  - `od1c_echo.exe` → 使い方を出力して終了（exit code 2）。
  - `od1c_echo.exe --echo こんにちは` → `こんにちは` と1行出力、exit code 0。
  - `od1c_echo.exe --repeat 3 文字列` → `文字列` を3行出力、exit code 0。
  - 不正引数 → エラーメッセージを標準エラーに出し、exit code 2。

### 入出力
- 入力：コマンドライン引数
- 出力：標準出力（必要に応じて標準エラー）

---

## 進め方ヒント
- **課題A**は以下のAPI/構造体を使う：`WNDCLASSEXW`, `CreateWindowExW`, `ShowWindow`, `UpdateWindow`, `GetMessageW`, `TranslateMessage`, `DispatchMessageW`, `DefWindowProcW`。
- **課題B**は以下を使う：`GetCommandLineW`, `CommandLineToArgvW`, `GetStdHandle`, `GetFileType`, `WriteConsoleW`, `WriteFile`。

---

## ビルド要件（抜粋）
- MinGW-w64（g++）例：
  - 課題A：`g++ -std=c++17 win_blank.cpp -municode -mwindows -o win_blank.exe`
  - 課題B：`g++ -std=c++17 od1c_echo.cpp -municode -o od1c_echo.exe -lshell32`
- MSVC（cl）例：
  - 課題A：`cl /std:c++17 /DUNICODE /D_UNICODE win_blank.cpp /link /SUBSYSTEM:WINDOWS`
  - 課題B：`cl /std:c++17 /DUNICODE /D_UNICODE od1c_echo.cpp shell32.lib`

> VS Code での `tasks.json` や詳細は同梱の **実装ガイド** を参照。

---

## 提出物
- `win_blank.exe` の実行結果スクショ（ウィンドウが中央に出た状態）。
- `od1c_echo.exe` の動作ログ（`--echo` と `--repeat` の2パターン）。
- 主要ソース（`win_blank.cpp`, `od1c_echo.cpp`）。

---

## 追加ポイント（任意）
- 課題A：DPI対応（Per-Monitor v2）や Alt+Enter で最大化/復帰などのショートカット対応。
- 課題B：`--help`, `--version`、エラーハンドリングの充実、UTF-8/UTF-16 変換関数の自作。

**楽しんで、堅実に。Win32は地味に奥が深い。**
