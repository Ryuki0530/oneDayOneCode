#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "app.h"
#include "tray.h"

// 隠しウィンドウ用のクラス名
static const char *APP_CLASS_NAME = "HotMemoHiddenWindowClass";

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // トレイアイコンを追加
            if (!Tray_Init(hwnd)) {
                MessageBoxA(NULL, "Failed to initialize tray icon.", "HotMemo", MB_ICONERROR);
                PostQuitMessage(1);
            }
            return 0;
        }

        case APP_WM_TRAY: {
            // トレイからの通知（右クリック等）
            Tray_HandleMessage(hwnd, wParam, lParam);
            return 0;
        }

        case WM_COMMAND: {
            // トレイメニュー選択はここに飛んでくる
            switch (LOWORD(wParam)) {
                case IDM_TRAY_EXIT:
                    DestroyWindow(hwnd); // WM_DESTROY へ
                    return 0;
                default:
                    break;
            }
            break;
        }

        case WM_DESTROY: {
            // 終了時にアイコン削除（幽霊化対策）
            Tray_Destroy(hwnd);
            PostQuitMessage(0);
            return 0;
        }

        default:
            break;
    }

    return DefWindowProcA(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    (void)hPrevInstance;
    (void)lpCmdLine;
    (void)nShowCmd;

    WNDCLASSEXA wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = APP_CLASS_NAME;

    // いちおうアイコン/カーソル（無くても動く）
    wc.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursorA(NULL, IDC_ARROW);

    if (!RegisterClassExA(&wc)) {
        MessageBoxA(NULL, "RegisterClassEx failed.", "HotMemo", MB_ICONERROR);
        return 1;
    }

    // 表に出さない「隠しウィンドウ」を作る（メッセージ受信用）
    HWND hwnd = CreateWindowExA(
        0,
        APP_CLASS_NAME,
        "HotMemoHiddenWindow",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hwnd) {
        MessageBoxA(NULL, "CreateWindowEx failed.", "HotMemo", MB_ICONERROR);
        return 1;
    }

    // 表示しない（常駐なので）
    // ShowWindow(hwnd, SW_SHOW); // ← Stage1では呼ばない

    MSG msg;
    while (GetMessageA(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return (int)msg.wParam;
}