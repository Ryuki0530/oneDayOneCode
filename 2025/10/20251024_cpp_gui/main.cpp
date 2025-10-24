#define UNICODE
#include <windows.h>
#include <iostream>

const wchar_t CLASS_NAME[] = L"Sample Window Class";
const wchar_t WINDOW_TITLE[] = L"Sample C++ GUI Application";
const int ID_BUTTON = 1;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            CreateWindowW(
                L"BUTTON", L"Click Me", 
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_BORDER, 
                50, 50, 100, 30, 
                hwnd, (HMENU)ID_BUTTON, 
                (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), 
                NULL
            );
            return 0;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == ID_BUTTON) {
                MessageBoxW(
                    hwnd,
                    L"Button Clicked!",
                    L"Notification",
                    MB_OK | MB_ICONINFORMATION
                );
                std::cout << "Button Clicked!";
            }
            return 0;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    // ウィンドウクラスの登録
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassW(&wc);

    // ウィンドウ作成
    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"課題ウィンドウ",
        WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
        return 0;

    ShowWindow(hwnd, nCmdShow);

    // メッセージループ
    MSG msg = {};
    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}