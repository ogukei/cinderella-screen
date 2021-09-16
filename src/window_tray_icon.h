
#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <optional>

namespace imascs {

// @see https://docs.microsoft.com/en-us/windows/win32/api/shellapi/ns-shellapi-notifyicondataa
class WindowTrayIcon {
 private:
  std::optional<GUID> guid_;
 public:
  WindowTrayIcon();
  WindowTrayIcon(const WindowTrayIcon&) = delete;

  static WindowTrayIcon& Instance() {
    static WindowTrayIcon instance;
    return instance;
  }

  void AddToSystemTray(
      HINSTANCE hInstance,
      HWND hwnd,
      UINT uCallbackMessage,
      LPCWSTR lpIconName,
      LPCWSTR lpTip);
  void RemoveFromSystemTray();
};

}  // namespace imascs
