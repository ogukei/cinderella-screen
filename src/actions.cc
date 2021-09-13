
#include "actions.h"

#include <iostream>

#include "defines.hpp"
#include "window_helper.h"
#include "fullscreen.h"

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
}

void MakeTargetAppBackground() {
  DWORD pid = FindProcessIDByName(IMASCS_TARGET_PROCESS_NAME);
  if (pid == 0) return;
  HWND window = FindMainWindow(pid);
  if (window == NULL) return;
  ConfigureBorderless(window);
  ConfigureFullscreen(window);
  ConfigureWallpaperWindow(window);
}

void MakeTargetAppDefault() {
  MakeTargetAppForeground();
  DWORD pid = FindProcessIDByName(IMASCS_TARGET_PROCESS_NAME);
  if (pid == 0) return;
  HWND window = FindMainWindow(pid);
  if (window == NULL) return;
  ConfigureBorderless(window);
  ConfigureDefaultWindow(window);
  ConfigureDefaultBorder(window);
}

}  // namespace imascs