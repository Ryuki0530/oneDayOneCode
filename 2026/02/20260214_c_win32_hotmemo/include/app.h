#ifndef APP_H
#define APP_H

// Windows API
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// トレイ通知用の独自メッセージ（WM_APP）
#define APP_WM_TRAY (WM_APP + 1)

// トレイメニューのコマンドID
#define IDM_TRAY_EXIT 1001

// トレイアイコン識別子
#define TRAY_ICON_ID 1

#endif // APP_H