#include "actions.h"

#include <iostream>

#include "defines.hpp"
#include "window_helper.h"
#include "fullscreen.h"
#include "app_state.h"

#include "core/winrt_context.h"
#include "capture/capture.h"

namespace imascs {

void MakeTargetAppForeground() {
  HWND window = FindWallpaperWindow(IMASCS_TARGET_WINDOW_CAPTION);
  if (window == NULL) return;
  SetParent(window, NULL);
  SetWindowPos(window, HWND_TOP, 0, 0, 0, 0,
      SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void MakeTargetAppFullscreen() {
  MakeTargetAppForeground();
  DWORD pid = FindProcessIDByName(IMASCS_TARGET_PROCESS_NAME);
  if (pid == 0) return;
  HWND window = FindMainWindow(pid);
  if (window == NULL) return;
  ConfigureBorderless(window);
  ConfigureFullscreen(window);
  if (GetForegroundWindow() != window) {
    SetForegroundWindow(window);
  }
  // hide cursor on idle
  auto& app_state = AppState::Instance().value();
  app_state->ActivateIdleCursorWatcher();
}

void MakeTargetAppBackground() {
  auto& app_state = AppState::Instance().value();
  app_state->DeactivateIdleCursorWatcher();
  DWORD pid = FindProcessIDByName(IMASCS_TARGET_PROCESS_NAME);
  if (pid == 0) return;
  HWND window = FindMainWindow(pid);
  if (window == NULL) return;
  ConfigureBorderless(window);
  ConfigureFullscreen(window);
  ConfigureWallpaperWindow(window);
}

void MakeTargetAppPictureInPicture() {
  auto& app_state = AppState::Instance().value();
  app_state->DeactivateIdleCursorWatcher();
  MakeTargetAppForeground();
  DWORD pid = FindProcessIDByName(IMASCS_TARGET_PROCESS_NAME);
  if (pid == 0) return;
  HWND target_window = FindMainWindow(pid);
  if (target_window == NULL) return;
  // Make window TOPMOST
  SetWindowPos(target_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void MakeTargetAppDefault() {
  auto& app_state = AppState::Instance().value();
  app_state->DeactivateIdleCursorWatcher();
  MakeTargetAppForeground();
  DWORD pid = FindProcessIDByName(IMASCS_TARGET_PROCESS_NAME);
  if (pid == 0) return;
  HWND window = FindMainWindow(pid);
  if (window == NULL) return;
  // Remove the TOPMOST setting
  SetWindowPos(window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
  // Restore other configurations
  ConfigureBorderless(window);
  ConfigureDefaultWindow(window);
  ConfigureDefaultBorder(window);
  // Bring the window to the foreground if it's not already
  if (GetForegroundWindow() != window) {
    SetForegroundWindow(window);
  }
}

void SnapshotTargetAppWindow() {
  DWORD pid = FindProcessIDByName(IMASCS_TARGET_PROCESS_NAME);
  if (pid == 0) return;
  HWND window = FindMainWindow(pid);
  if (window == NULL) return;
  auto& winrt_context = core::WinRTContext::Instance();
  auto& capture_facade = imascs::capture::CaptureFacade::Instance(winrt_context);
  capture_facade.SnapshotWindow(window);
}

void LaunchScreenshotFolder() {
  auto& winrt_context = core::WinRTContext::Instance();
  auto& capture_facade = imascs::capture::CaptureFacade::Instance(winrt_context);
  capture_facade.LaunchScreenshotFolder();
}

}  // namespace imascs
