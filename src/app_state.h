
#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

#include <memory>
#include <optional>

namespace imascs {

class IdleCursorWatcher;

class AppState {
 private:
  HINSTANCE hInstance_;
  std::optional<HWND> hwnd_;

  std::optional<std::shared_ptr<IdleCursorWatcher>> idle_cursor_watcher_;

  static std::optional<std::unique_ptr<AppState>>& State() {
    static std::optional<std::unique_ptr<AppState>> state;
    return state;
  }

 public:
  explicit AppState(HINSTANCE hInstance);
  AppState(const AppState&) = delete;

  static void ConfigureInstance(HINSTANCE hInstance) {
    State() = std::make_unique<AppState>(hInstance);
  }

  static void DestroyInstance() {
    State() = std::nullopt;
  }

  static const std::optional<std::unique_ptr<AppState>>& Instance() {
    return AppState::State();
  }

  HINSTANCE HInstance() const { return hInstance_; }
  std::optional<HWND> HWnd() const { return hwnd_; }

  void ConfigureHWnd(HWND hwnd) { hwnd_ = hwnd; }

  void ActivateIdleCursorWatcher();
  void DeactivateIdleCursorWatcher();
};

}  // namespace imascs
