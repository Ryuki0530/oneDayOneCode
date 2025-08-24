// UTF-8 (BOMなし)で保存
#include <windows.h>
#include <commctrl.h>
#include <commdlg.h>  // GetOpenFileNameW

#include "wav_player.h"

// コントロールID
enum { ID_EDIT = 1001, ID_BTN_PLAY = 1002, ID_BTN_BROWSE = 1003, ID_BTN_STOP = 1004 };

static HWND g_hEdit = NULL;
static HWND g_hBtnPlay = NULL;
static HWND g_hBtnBrowse = NULL;
static HWND g_hBtnStop = NULL;

static BOOL OpenFileDialog(HWND owner, wchar_t *outPath, DWORD outCount) {
    // フィルタは「説明\0パターン\0…\0\0」の二重終端
    // 例：すべてのファイル / テキスト / 画像
    static const wchar_t FILTER[] =
        L"オーディオファイル (*.wav)\0*.wav\0";

    OPENFILENAMEW ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner   = owner;
    ofn.lpstrFilter = FILTER;
    ofn.lpstrFile   = outPath;   // ここに結果が入る
    ofn.nMaxFile    = outCount;  // バッファサイズ（wchar_t個）
    ofn.Flags       = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

    // 初期値クリア
    outPath[0] = L'\0';

    return GetOpenFileNameW(&ofn);
}

static void CreateUI(HWND hWnd) {
    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_STANDARD_CLASSES };
    InitCommonControlsEx(&icc);

    g_hEdit = CreateWindowExW(
        0, L"EDIT", L"",
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
        10, 10, 380, 28,
        hWnd, (HMENU)(INT_PTR)ID_EDIT, GetModuleHandleW(NULL), NULL);

    g_hBtnBrowse = CreateWindowExW(
        0, L"BUTTON", L"参照…",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        400, 10, 90, 28,
        hWnd, (HMENU)(INT_PTR)ID_BTN_BROWSE, GetModuleHandleW(NULL), NULL);

    g_hBtnPlay = CreateWindowExW(
        0, L"BUTTON", L"Play",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        10, 50, 80, 30,
        hWnd, (HMENU)(INT_PTR)ID_BTN_PLAY, GetModuleHandleW(NULL), NULL);
    
    g_hBtnStop = CreateWindowExW(
        0, L"BUTTON", L"Stop",
        WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        100, 50, 80, 30,
        hWnd, (HMENU)(INT_PTR)ID_BTN_STOP, GetModuleHandleW(NULL), NULL);
}

static void Layout(HWND hWnd) {
    RECT rc; GetClientRect(hWnd, &rc);
    int pad = 10;
    int w = rc.right - rc.left;

    int browseW = 90, h = 28;
    int editW = w - pad*3 - browseW;
    if (editW < 80) editW = 80;

    SetWindowPos(g_hEdit, NULL, pad, pad, editW, h, SWP_NOZORDER);
    SetWindowPos(g_hBtnBrowse, NULL, pad*2 + editW, pad, browseW, h, SWP_NOZORDER);
    SetWindowPos(g_hBtnPlay, NULL, pad, pad + h + 10, 100, 30, SWP_NOZORDER);
    SetWindowPos(g_hBtnStop, NULL, pad + 110, pad + h + 10, 100, 30, SWP_NOZORDER);
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        CreateUI(hWnd);
        return 0;

    case WM_SIZE:
        Layout(hWnd);
        return 0;

    case WM_COMMAND: {
        switch (LOWORD(wParam)) {
        case ID_BTN_BROWSE: {
            wchar_t path[MAX_PATH * 8];
            if (OpenFileDialog(hWnd, path, (DWORD)(sizeof(path)/sizeof(path[0])))) {
                SetWindowTextW(g_hEdit, path);
            }
            return 0;
        }
        case ID_BTN_PLAY: {
            wchar_t buf[1024];
            GetWindowTextW(g_hEdit, buf, 1024);
            if (buf[0]) {
                char mbbuf[1024];
                WideCharToMultiByte(CP_ACP, 0, buf, -1, mbbuf, sizeof(mbbuf), NULL, NULL);
                wav_play_start(mbbuf);
            } else {
                MessageBoxW(hWnd, L"(空です)", L"選択されたパス", MB_OK | MB_ICONINFORMATION);
            }
            return 0;
        }
        case ID_BTN_STOP: {
            wav_play_stop();
            return 0;
        }
        }
        break; // ← ここでswitch(LOWORD(wParam))を抜ける
    } // ← これが必要！WM_COMMANDのcaseブロックを閉じる

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow) {
    (void)hPrev; (void)pCmdLine; // 未使用抑制

    // （任意）DPI対応：Win10以降なら効果あり
    HMODULE hUser32 = LoadLibraryW(L"User32.dll");
    if (hUser32) {
        typedef BOOL (WINAPI *PFN_SetProcessDpiAwarenessContext)(HANDLE);
        PFN_SetProcessDpiAwarenessContext p =
            (PFN_SetProcessDpiAwarenessContext)GetProcAddress(hUser32, "SetProcessDpiAwarenessContext");
        if (p) p((HANDLE)-4); // PER_MONITOR_AWARE_V2
        FreeLibrary(hUser32);
    }

    const wchar_t *kClassName = L"RyukiWin32FileDialog";
    WNDCLASSEXW wc = { 0 };
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = kClassName;
    RegisterClassExW(&wc);

    HWND hWnd = CreateWindowExW(
        WS_EX_APPWINDOW,
        kClassName, L"WAV Player",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 560, 140,
        NULL, NULL, hInst, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}
