
#include "window_tray_icon.h"

#include <iostream>

#include "window_helper.h"

namespace imascs {

WindowTrayIcon::WindowTrayIcon() {
  GUID guid;
  if (CoCreateGuid(&guid) == S_OK) {
    guid_ = guid;
  }
}

void WindowTrayIcon::AddToSystemTray(
    HINSTANCE hInstance,
    HWND hwnd,
    UINT uCallbackMessage,
    LPCWSTR lpIconName,
    LPCWSTR lpTip) 
{
  if (!guid_) return;
  RegisterTrayIcon(hInstance, hwnd, guid_.value(),
      uCallbackMessage, lpIconName, lpTip);
}

void WindowTrayIcon::RemoveFromSystemTray() {
  if (!guid_) return;
  UnregisterTrayIcon(guid_.value());
}

}  // namespace imascs
