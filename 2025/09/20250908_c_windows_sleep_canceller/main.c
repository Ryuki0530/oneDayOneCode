#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <shellapi.h>
#include <stdbool.h>

#define APP_CLASS      L"NoSleepTrayWindow"
#define WM_TRAYICON    (WM_APP + 1)
#define TRAY_UID       1
#define ID_TRAY_ENABLE 1001
#define ID_TRAY_EXIT   1002

static NOTIFYICONDATAW nid = {0};
static bool g_enabled = false;
static HINSTANCE g_hInst;
static HWND g_hWnd;

static void ApplyExecutionState(bool enable) {
    if (enable) {
        SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED | ES_AWAYMODE_REQUIRED);
    } else {
        SetThreadExecutionState(ES_CONTINUOUS);
    }
}

static void UpdateTrayTipAndBalloon(HWND hWnd) {
    (void)hWnd; 
    const wchar_t* tip = g_enabled ? L"NOSLEEP: 有効 (左クリックで無効)" : L"NOSLEEP: 無効 (左クリックで有効)";
    wcsncpy(nid.szTip, tip, sizeof(nid.szTip)/sizeof(wchar_t) - 1);

    // Balloon
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
    const wchar_t* infoTitle = L"NOSLEEP";
    const wchar_t* infoText  = g_enabled ? L"スリープ/画面オフを抑止中" : L"抑止を解除しました";
    wcsncpy(nid.szInfoTitle, infoTitle, sizeof(nid.szInfoTitle)/sizeof(wchar_t)-1);
    wcsncpy(nid.szInfo, infoText, sizeof(nid.szInfo)/sizeof(wchar_t)-1);
    nid.dwInfoFlags = g_enabled ? NIIF_INFO : NIIF_NONE;
    Shell_NotifyIconW(NIM_MODIFY, &nid);
}

static void EnsureTrayIcon(HWND hWnd) {
    nid.cbSize = sizeof(nid);
    nid.hWnd = hWnd;
    nid.uID = TRAY_UID;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    const wchar_t* tip = L"NOSLEEP: 無効 (左クリックで有効)";
    wcsncpy(nid.szTip, tip, sizeof(nid.szTip)/sizeof(wchar_t) - 1);
    Shell_NotifyIconW(NIM_ADD, &nid);
}

static void RemoveTrayIcon(void) {
    if (nid.hWnd) {
        Shell_NotifyIconW(NIM_DELETE, &nid);
    }
}

static void ToggleEnable(HWND hWnd) {
    g_enabled = !g_enabled;
    ApplyExecutionState(g_enabled);
    UpdateTrayTipAndBalloon(hWnd);
}

static void ShowContextMenu(HWND hWnd) {
    POINT pt;
    GetCursorPos(&pt);
    HMENU hMenu = CreatePopupMenu();
    if (!hMenu) return;

    AppendMenuW(hMenu, MF_STRING, ID_TRAY_ENABLE, g_enabled ? L"無効にする" : L"有効にする");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, ID_TRAY_EXIT, L"終了");

    // 必要：フォアグラウンドで表示しないとメニューが閉じないことがある
    SetForegroundWindow(hWnd);
    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
    DestroyMenu(hMenu);
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        EnsureTrayIcon(hWnd);
        return 0;
    case WM_TRAYICON:
        if (LOWORD(wParam) == TRAY_UID) {
            if (lParam == WM_LBUTTONUP || lParam == WM_LBUTTONDBLCLK) {
                ToggleEnable(hWnd);
            } else if (lParam == WM_RBUTTONUP) {
                ShowContextMenu(hWnd);
            }
        }
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_TRAY_ENABLE:
            ToggleEnable(hWnd);
            break;
        case ID_TRAY_EXIT:
            PostMessageW(hWnd, WM_CLOSE, 0, 0);
            break;
        }
        return 0;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        return 0;
    case WM_DESTROY:
        ApplyExecutionState(false); // 元に戻す
        RemoveTrayIcon();
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrev, PWSTR lpCmdLine, int nShowCmd) {
    (void)hPrev; (void)lpCmdLine; (void)nShowCmd;
    g_hInst = hInst;

    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = APP_CLASS;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClassW(&wc)) {
        MessageBoxW(NULL, L"ウィンドウクラス登録に失敗", L"NOSLEEP", MB_ICONERROR);
        return 1;
    }

    g_hWnd = CreateWindowExW(0, APP_CLASS, L"NOSLEEP", 0,
                             CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                             NULL, NULL, hInst, NULL);
    if (!g_hWnd) {
        MessageBoxW(NULL, L"ウィンドウ生成に失敗", L"NOSLEEP", MB_ICONERROR);
        return 1;
    }

    // 表示しない（トレイ常駐）
    // ShowWindow(g_hWnd, SW_HIDE);

    // 初期状態は無効
    ApplyExecutionState(false);

    MSG msg;
    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}
