#ifndef HOTKEY_H
#define HOTKEY_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// Ctrl+Alt+M を登録
BOOL Hotkey_Register(HWND hwnd);

// 登録解除
void Hotkey_Unregister(HWND hwnd);

#ifdef __cplusplus
}
#endif

#endif // HOTKEY_H