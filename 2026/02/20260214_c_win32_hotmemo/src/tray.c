#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>   // Shell_NotifyIcon
#include "app.h"
#include "tray.h"

static NOTIFYICONDATAA g_nid;  // 今回は A(ANSI) で統一（まず動くこと優先）

static void Tray_ShowContextMenu(HWND hwnd) {
    HMENU menu = CreatePopupMenu();
    if (!menu) return;

    AppendMenuA(menu, MF_STRING, IDM_TRAY_EXIT, "Exit");

    // ウィンドウ最前面に。
    // これをしないと、タスクバーの下に隠れることがあるらしい。
    SetForegroundWindow(hwnd);

    POINT pt;
    GetCursorPos(&pt);

    TrackPopupMenu(
        menu,
        TPM_RIGHTBUTTON | TPM_BOTTOMALIGN | TPM_LEFTALIGN,
        pt.x, pt.y,
        0,
        hwnd,
        NULL
    );

    DestroyMenu(menu);
}

BOOL Tray_Init(HWND hwnd) {
    ZeroMemory(&g_nid, sizeof(g_nid));
    g_nid.cbSize = sizeof(g_nid);
    g_nid.hWnd = hwnd;
    g_nid.uID = TRAY_ICON_ID;
    g_nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    g_nid.uCallbackMessage = APP_WM_TRAY;

    //とりあえず既定の情報アイコン（.ico不要）
    g_nid.hIcon = LoadIconA(NULL, IDI_INFORMATION);

    lstrcpynA(g_nid.szTip, "HotMemo (Stage1)", (int)sizeof(g_nid.szTip));

    return Shell_NotifyIconA(NIM_ADD, &g_nid);
}

void Tray_Destroy(HWND hwnd) {
    (void)hwnd;
    Shell_NotifyIconA(NIM_DELETE, &g_nid);
}

void Tray_HandleMessage(HWND hwnd, WPARAM wParam, LPARAM lParam) {
    // wParam: アイコンID（uID）が来る
    if ((UINT)wParam != TRAY_ICON_ID) return;

    switch ((UINT)lParam) {
        case WM_RBUTTONUP:
            Tray_ShowContextMenu(hwnd);
            break;

        // 左クリック
        default:
            break;
    }
}