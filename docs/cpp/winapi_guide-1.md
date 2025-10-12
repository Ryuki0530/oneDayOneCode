
# C++ × Windows API 実装ガイド（2025-10-12）

このドキュメントは、**課題A（無地のWin32ウィンドウ）** と **課題B（コマンドライン出力ツール）** を VS Code + Windows 11 で実装するための**実用ガイド**です。

---

## 1. プロジェクト構成（例）
```
od1c_winapi/
├─ win_blank.cpp      # 課題A
├─ od1c_echo.cpp      # 課題B
└─ .vscode/
   ├─ tasks.json
   └─ launch.json     # 任意
```

---

## 2. 共通の前提
- **Unicode** 前提で進めます。コンパイル時に `UNICODE`, `_UNICODE` を有効化してください。
- 文字列リテラルは `L"..."`、APIは `...W` サフィックスを使用。

---

## 3. 課題A：無地ウィンドウの最小構成
### 3.1 必須コンポーネント
- `WinMain`（GUIサブシステムのエントリ）
- `WNDCLASSEXW`（ウィンドウクラス登録）
- `CreateWindowExW`（ウィンドウ生成）
- `ShowWindow` / `UpdateWindow`（表示）
- メッセージループ：`GetMessageW` → `TranslateMessage` → `DispatchMessageW`
- `WndProc`（ウィンドウプロシージャ）で `WM_DESTROY` に `PostQuitMessage(0)`

### 3.2 サンプル（抜粋）
```cpp
// win_blank.cpp
#define UNICODE
#define _UNICODE
#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int nCmdShow) {
    const wchar_t* CLASS_NAME = L"OD1CClass";
    WNDCLASSEXW wc{ sizeof(wc) };
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = nullptr;
    wc.lpszClassName = CLASS_NAME;
    wc.hIconSm       = wc.hIcon;
    RegisterClassExW(&wc);

    // 800x600 を画面中央へ
    int width = 800, height = 600;
    RECT r{ 0, 0, width, height };
    AdjustWindowRect(&r, WS_OVERLAPPEDWINDOW, FALSE);
    int W = r.right - r.left;
    int H = r.bottom - r.top;
    int scrW = GetSystemMetrics(SM_CXSCREEN);
    int scrH = GetSystemMetrics(SM_CYSCREEN);
    int X = (scrW - W) / 2;
    int Y = (scrH - H) / 2;

    HWND hWnd = CreateWindowExW(
        0, CLASS_NAME, L"OD1C Window - 2025-10-12",
        WS_OVERLAPPEDWINDOW, X, Y, W, H,
        nullptr, nullptr, hInst, nullptr
    );

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}
```

### 3.3 ビルド
- **MinGW-w64 (g++)**
  ```bash
  g++ -std=c++17 win_blank.cpp -municode -mwindows -o win_blank.exe
  ```
- **MSVC (cl)**
  ```bat
  cl /std:c++17 /DUNICODE /D_UNICODE win_blank.cpp /link /SUBSYSTEM:WINDOWS
  ```

> `-mwindows`（MinGW）や `/SUBSYSTEM:WINDOWS`（MSVC）で **コンソールなし** の GUI 実行体になります。

---

## 4. 課題B：コマンドライン出力（Unicode/リダイレクト安全）
### 4.1 ねらい
- 日本語を含む引数を **確実に取得**（`GetCommandLineW` → `CommandLineToArgvW`）。
- 出力先が **端末かリダイレクトか**を判定して適切に出力（`WriteConsoleW` or `WriteFile`）。

### 4.2 サンプル（抜粋）
```cpp
// od1c_echo.cpp
#define UNICODE
#define _UNICODE
#include <windows.h>
#include <shellapi.h>   // CommandLineToArgvW
#include <string>
#include <vector>

static bool is_console(HANDLE h) {
    DWORD ft = GetFileType(h);
    if (ft == FILE_TYPE_CHAR) return true;
    // パイプ/ファイルは false 扱い
    return false;
}

static void write_line(HANDLE h, const std::wstring& s) {
    DWORD written = 0;
    if (is_console(h)) {
        WriteConsoleW(h, s.c_str(), (DWORD)s.size(), &written, nullptr);
        WriteConsoleW(h, L"\r\n", 2, &written, nullptr);
    } else {
        // UTF-8 にして出す実装も可。簡略化のため BOM なし UTF-16LE を投げる例
        std::wstring with_nl = s + L"\r\n";
        WriteFile(h, with_nl.c_str(), (DWORD)(with_nl.size() * sizeof(wchar_t)), &written, nullptr);
    }
}

int wmain() {
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!argv || argc <= 1) {
        HANDLE err = GetStdHandle(STD_ERROR_HANDLE);
        write_line(err, L"Usage: od1c_echo --echo <text> | --repeat <n> <text>");
        return 2;
    }

    std::wstring cmd = argv[1];
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE err = GetStdHandle(STD_ERROR_HANDLE);

    if (cmd == L"--echo" && argc >= 3) {
        std::wstring text = argv[2];
        write_line(out, text);
        return 0;
    } else if (cmd == L"--repeat" && argc >= 4) {
        int n = _wtoi(argv[2]);
        if (n <= 0) { write_line(err, L"error: n must be > 0"); return 2; }
        std::wstring text = argv[3];
        for (int i = 0; i < n; ++i) write_line(out, text);
        return 0;
    } else {
        write_line(err, L"error: invalid arguments");
        write_line(err, L"Usage: od1c_echo --echo <text> | --repeat <n> <text>");
        return 2;
    }
}
```

### 4.3 ビルド
- **MinGW-w64 (g++)**
  ```bash
  g++ -std=c++17 od1c_echo.cpp -municode -o od1c_echo.exe -lshell32
  ```
- **MSVC (cl)**
  ```bat
  cl /std:c++17 /DUNICODE /D_UNICODE od1c_echo.cpp shell32.lib
  ```

> 端末直書きは `WriteConsoleW`、パイプ/ファイルへは `WriteFile` を使用。UTF-8 出力にしたい場合は `WideCharToMultiByte(CP_UTF8, ...)` で変換してから `WriteFile`。

---

## 5. VS Code 設定（MinGW 例）
`.vscode/tasks.json`（最小）
```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "build win_blank (GUI)",
      "type": "shell",
      "command": "g++",
      "args": ["-std=c++17", "win_blank.cpp", "-municode", "-mwindows", "-o", "win_blank.exe"]
    },
    {
      "label": "build od1c_echo (console)",
      "type": "shell",
      "command": "g++",
      "args": ["-std=c++17", "od1c_echo.cpp", "-municode", "-o", "od1c_echo.exe", "-lshell32"]
    }
  ]
}
```

---

## 6. 便利トピック
- **GUIアプリに一時的にコンソールを出す**：
  ```cpp
  AllocConsole();
  FILE* fp; freopen_s(&fp, "CONOUT$", "w", stdout); // printf も使える
  ```
- **DPI 対応（ヒント）**：`SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);`
- **メッセージポンプの落とし穴**：`PeekMessage` でのビジーループ、`SendMessage` の再入など。

---

## 7. 動作確認例
- `win_blank.exe` を実行 → 中央に 800×600 の無地ウィンドウが出ること。
- `od1c_echo.exe --echo こんにちは` → 端末に `こんにちは` が1行表示されること。
- `od1c_echo.exe --repeat 3 テスト > out.txt` → `out.txt` に3行書き出されること。

---

## 8. 仕上げのチェックリスト
- [ ] UNICODE 定義が有効。
- [ ] 例外/エラー時の終了コードが妥当。
- [ ] GUI の WM_ 系イベントで暴走がない。
- [ ] リダイレクト時の文字化けがない（必要なら UTF-8 変換）。

Win32は静かな職人芸。APIの織り目を理解すると GUI も CLI も自在に操れます。
