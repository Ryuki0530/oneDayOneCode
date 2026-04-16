#ifndef TRAY_H
#define TRAY_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

// トレイ初期化（アイコン追加）
BOOL Tray_Init(HWND hwnd);

// トレイ終了（アイコン削除）
void Tray_Destroy(HWND hwnd);

// トレイ通知メッセージ（右クリックとか）
void Tray_HandleMessage(HWND hwnd, WPARAM wParam, LPARAM lParam);

#ifdef __cplusplus
}
#endif

#endif // TRAY_H