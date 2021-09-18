
#include "hotkeys.h"

#include <iostream>

#include "defines.hpp"
#include "actions.h"
#include "window_helper.h"

namespace imascs {

static bool IsProcessWindowActive() {
  HWND hwnd = GetForegroundWindow();
  DWORD dwProcessId = 0;
  GetWindowThreadProcessId(hwnd, &dwProcessId);
  return dwProcessId == GetCurrentProcessId();
}

static bool IsTargetWindowActive() {
  HWND wallpaper = FindWallpaperWindow(IMASCS_TARGET_WINDOW_CAPTION);
  if (wallpaper != NULL) return true;
  DWORD dwTargetProcessId = FindProcessIDByName(IMASCS_TARGET_PROCESS_NAME);
  if (dwTargetProcessId == 0) return false;
  DWORD dwWindowProcessId = 0;
  GetWindowThreadProcessId(GetForegroundWindow(), &dwWindowProcessId);
  return dwWindowProcessId == dwTargetProcessId;
}

static void DispatchActions(DWORD vkCode) {
  switch (vkCode) {
  case VK_F12:
    SnapshotTargetAppWindow();
    break;
  case VK_F11:
    MakeTargetAppFullscreen();
    break;
  case VK_F10:
    MakeTargetAppBackground();
    break;
  case VK_F9:
    MakeTargetAppDefault();
    break;
  }
}

LRESULT CALLBACK HotkeysHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
  LPKBDLLHOOKSTRUCT hook = (LPKBDLLHOOKSTRUCT)lParam;
  if (wParam == WM_KEYDOWN && nCode == HC_ACTION) {
    switch (hook->vkCode) {
    case VK_F12:
    case VK_F11:
    case VK_F10:
    case VK_F9:
      if (IsTargetWindowActive() || IsProcessWindowActive()) {
        DispatchActions(hook->vkCode);
      }
      break;
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void ConfigureGlobalHotkeys() {
  SetWindowsHookEx(WH_KEYBOARD_LL, HotkeysHookProc, NULL, 0);
}

}  // namespace imascs
