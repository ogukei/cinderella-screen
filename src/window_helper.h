
#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

namespace imascs {

DWORD FindProcessIDByName(LPCWSTR name);
HWND FindMainWindow(DWORD pid);
bool FetchWindowMonitorRect(HWND hwnd, RECT* out);
bool FetchMonitorRect(HMONITOR hMonitor, RECT* out);

void ConfigureBorderless(HWND hwnd);
void ConfigureDefaultBorder(HWND hwnd);

BOOL EnablePrivilege(LPCWSTR lpPrivilegeName);

static BOOL EnableSeDebugPrivilege() {
  return EnablePrivilege(SE_DEBUG_NAME);
}

DWORD GetOverlappedWindowStyle();
HWND MakeOverlappedWindow(HINSTANCE hInstance,
    LPCWSTR lpClassName,
    LPCWSTR lpTitle,
    int width, int height);

HWND MakeButton(HINSTANCE hInstance, HWND hwnd, 
    int x, int y, int width, int height,
    LPWSTR lpWindowName, HMENU hMenu);

void RegisterWindowClass(HINSTANCE hInstance,
    LPCWSTR lpClassName,
    LPCWSTR lpMenuName,
    LPCWSTR lpIconName,
    WNDPROC lpfnWndProc);
void ShowContextMenu(HINSTANCE hInstance,
    HWND hwnd,
    LPCWSTR lpMenuName,
    POINT pt);
void RegisterTrayIcon(HINSTANCE hInstance,
    HWND hwnd,
    GUID guid,
    UINT uCallbackMessage,
    LPCWSTR lpIconName,
    LPCWSTR lpTip);
void UnregisterTrayIcon(GUID guid);

void ConfigureWallpaperWindow(HWND hwnd);
HWND FindWallpaperWindow(LPWSTR lpWindowCaption);

}  // namespace imascs
