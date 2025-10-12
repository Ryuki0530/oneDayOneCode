// win_blank.cpp
#include <windows.h>

const wchar_t CLASS_NAME[] = L"OD1CWindowClass";
const wchar_t WINDOW_TITLE[] = L"OD1C Window - 2025-10-12";
const int WIN_WIDTH = 800;
const int WIN_HEIGHT = 600;

// 単色背景色（白）
const COLORREF BG_COLOR = RGB(255, 255, 255);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_ERASEBKGND: {
        HDC hdc = (HDC)wParam;
        RECT rc;
        GetClientRect(hwnd, &rc);
        HBRUSH hBrush = CreateSolidBrush(BG_COLOR);
        FillRect(hdc, &rc, hBrush);
        DeleteObject(hBrush);
        return 1;
    }
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int nCmdShow) {
    WNDCLASSEXW wc = { sizeof(wc) };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = nullptr; // WM_ERASEBKGNDで塗る
    wc.lpszClassName = CLASS_NAME;
    RegisterClassExW(&wc);

    // 画面中央に配置
    int scrW = GetSystemMetrics(SM_CXSCREEN);
    int scrH = GetSystemMetrics(SM_CYSCREEN);
    int x = (scrW - WIN_WIDTH) / 2;
    int y = (scrH - WIN_HEIGHT) / 2;

    HWND hwnd = CreateWindowExW(
        0, CLASS_NAME, WINDOW_TITLE,
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX, // サイズ変更不可
        x, y, WIN_WIDTH, WIN_HEIGHT,
        nullptr, nullptr, hInst, nullptr
    );
    if (!hwnd) return 1;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}