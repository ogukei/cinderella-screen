
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

#include <iostream>
#include <string>

#include "console.hpp"
#include "defines.hpp"

#include "resource.h"
#include "actions.h"
#include "hotkeys.h"
#include "window_helper.h"
#include "ui.h"

#define WMAPP_NOTIFYCALLBACK (WM_APP + 1)

static HINSTANCE g_hInstance;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
#ifdef _DEBUG
  ConsoleActivationGuard console;
#endif
  g_hInstance = hInstance;
  // create window
  imascs::RegisterWindowClass(hInstance, 
      IMASCS_WINDOW_CLASS_NAME,
      MAKEINTRESOURCE(IDC_NOTIFICATIONICON),
      MAKEINTRESOURCE(IDI_ICON1),
      WindowProc);
  SIZE size = imascs::GetUIBoundingBox();
  HWND hwnd = imascs::MakeOverlappedWindow(hInstance,
      IMASCS_WINDOW_CLASS_NAME, IMASCS_WINDOW_TITLE,
      size.cx, size.cy);
  if (hwnd == NULL) return 0;
  ShowWindow(hwnd, nCmdShow);
  // message loop
  MSG msg = {};
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return 0;
}

static bool DispatchCommandActions(HWND hwnd, WORD word) {
  switch (word) {
  case IDM_EXIT:
    DestroyWindow(hwnd);
    break;
  case IDM_ACTION1:
    imascs::MakeTargetAppFullscreen();
    break;
  case IDM_ACTION2:
    imascs::MakeTargetAppBackground();
    break;
  case IDM_ACTION3:
    imascs::MakeTargetAppDefault();
    break;
  default:
    return false;
  }
  return true;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  static const GUID guid = 
      {0xe1e78020, 0x8952, 0x4227, {0xb8, 0xf3, 0xd2, 0x4f, 0xd4, 0xf6, 0x5b, 0x6c}};
  switch (uMsg) {
  case WM_CREATE:
    imascs::RegisterTrayIcon(g_hInstance, hwnd, guid,
        WMAPP_NOTIFYCALLBACK, MAKEINTRESOURCE(IDI_ICON1), IMASCS_TRAY_TOOLTIP);
    imascs::SetupUI(g_hInstance, hwnd);
    imascs::ConfigureGlobalHotkeys();
    return 0;
  case WM_DESTROY:
    imascs::UnregisterTrayIcon(guid);
    PostQuitMessage(0);
    return 0;
  case WM_COMMAND:
    if (DispatchCommandActions(hwnd, LOWORD(wParam))) {
      return 0;
    }
    break;
  case WMAPP_NOTIFYCALLBACK:
    switch (LOWORD(lParam)) {
    case WM_CONTEXTMENU:
      POINT pt = {LOWORD(wParam), HIWORD(wParam)};
      imascs::ShowContextMenu(g_hInstance, hwnd, MAKEINTRESOURCE(IDC_NOTIFICATIONICON), pt);
      return 0;
    }
    break;
  default:
    break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
