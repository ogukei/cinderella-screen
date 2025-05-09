
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
#include "window_tray_icon.h"
#include "ui.h"
#include "app_state.h"

#define WMAPP_NOTIFYCALLBACK (WM_APP + 1)

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
#ifdef _DEBUG
  ConsoleActivationGuard console;
#endif
  imascs::AppState::ConfigureInstance(hInstance);
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
  case IDM_ACTION4:
    imascs::SnapshotTargetAppWindow();
    break;
  case IDM_ACTION5:
    imascs::MakeTargetAppPictureInPicture();
    break;
  case IDM_HELP1:
    imascs::LaunchScreenshotFolder();
    break;
  default:
    return false;
  }
  return true;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
  case WM_CREATE:
  {
    auto& app_state = imascs::AppState::Instance().value();
    app_state->ConfigureHWnd(hwnd);
    imascs::WindowTrayIcon::Instance().AddToSystemTray(app_state->HInstance(),
      hwnd, WMAPP_NOTIFYCALLBACK, MAKEINTRESOURCE(IDI_ICON1), IMASCS_TRAY_TOOLTIP);
    imascs::SetupUI(app_state->HInstance(), hwnd);
    imascs::ConfigureGlobalHotkeys();
    return 0;
  }
  case WM_DESTROY:
    imascs::WindowTrayIcon::Instance().RemoveFromSystemTray();
    imascs::AppState::DestroyInstance();
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
      auto& app_state = imascs::AppState::Instance().value();
      POINT pt = {LOWORD(wParam), HIWORD(wParam)};
      imascs::ShowContextMenu(app_state->HInstance(), hwnd, MAKEINTRESOURCE(IDC_NOTIFICATIONICON), pt);
      return 0;
    }
    break;
  default:
    break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
