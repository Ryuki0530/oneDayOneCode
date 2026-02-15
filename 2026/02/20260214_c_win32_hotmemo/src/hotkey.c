#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "app.h"
#include "hotkey.h"

BOOL Hotkey_Register(HWND hwnd) {
    // MOD_NOREPEAT を付けると、押しっぱなしの連打が抑えられる（環境によっては未対応）
    // MinGWのヘッダに無い場合があるので、安全に 0 でもOK。
#ifndef MOD_NOREPEAT
#define MOD_NOREPEAT 0
#endif

    UINT modifiers = MOD_CONTROL | MOD_ALT | MOD_NOREPEAT;
    UINT vk = 'M';

    return RegisterHotKey(hwnd, HOTKEY_ID_MAIN, modifiers, vk);
}

void Hotkey_Unregister(HWND hwnd) {
    UnregisterHotKey(hwnd, HOTKEY_ID_MAIN);
}