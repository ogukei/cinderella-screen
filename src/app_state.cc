
#include "app_state.h"

#include <iostream>

#include "defines.hpp"
#include "idle_cursor.h" 

namespace imascs {

AppState::AppState(HINSTANCE hInstance)
  : hInstance_(hInstance)
{
}

void AppState::ActivateIdleCursorWatcher() {
  if (hwnd_.has_value()) {
    idle_cursor_watcher_ = IdleCursorWatcher::Create(hInstance_, hwnd_.value());
  }
}

void AppState::DeactivateIdleCursorWatcher() {
  idle_cursor_watcher_ = std::nullopt;
}

}  // namespace imascs
