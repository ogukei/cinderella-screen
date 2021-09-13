
#include "fullscreen.h"

#include <iostream>
#include <numeric>
#include <algorithm>

#include "window_helper.h"

namespace imascs {

static bool CheckAspectRatio16_9(long width, long height) {
  long d = std::gcd(width, height);
  long rw = width / d;
  long rh = height / d;
  return rw == 16 && rh == 9;
}

static bool IsLandscape(long width, long height) {
  return width >= height;
}

static void FixSizeToAspect16_9(long& width, long& height) {
  double w = static_cast<double>(width);
  double h = static_cast<double>(height);
  long r = 0;
  if ((w / h) < (16.0 / 9.0)) {
    r = width / 16;
    if ((width % 16) != 0) {
      r += 1;
    }
  } else {
    r = height / 9;
    if ((height % 9) != 0) {
      r += 1;
    }
  }
  w = static_cast<double>(r) * 16.0;
  h = static_cast<double>(r) * 9.0; 
  width = std::lround(w);
  height = std::lround(h);
}

static void CalcFullscreenSize(long& x, long& y, long& width, long& height,
    long monitor_width, long monitor_height,
    long client_width, long client_height)
{
  // check whether it is possible to fill client rect to the screen
  if (IsLandscape(client_width, client_height)) {
    // app is in landscape mode
    // if a monitor fits in 16:9, directly assign size
    bool is16_9 = CheckAspectRatio16_9(monitor_width, monitor_height);
    if (is16_9) {
      x = 0;
      y = 0;
      width = monitor_width;
      height = monitor_height;
      return;
    } else {
      // fallthrough
    }
  } else {
    // app is in portrait mode
    bool is9_16 = CheckAspectRatio16_9(monitor_height, monitor_width);
    if (is9_16) {
      x = 0;
      y = 0;
      width = monitor_width;
      height = monitor_height;
      return;
    } else {
      // fallthrough
    }
  }
  // some adjustment
  long target_width = monitor_width;
  long target_height = monitor_height;
  if (IsLandscape(client_width, client_height)) {
    // ensure 16:9
    FixSizeToAspect16_9(target_width, target_height);
  } else {
    // ensure 9:16
    FixSizeToAspect16_9(target_height, target_width);
  }
  // align horizontally center
  x = (monitor_width - target_width) / 2;
  y = (monitor_height - target_height) / 2;
  width = target_width;
  height = target_height;
  return;
}

static void CalcWindowFullscreenSize(HWND hwnd,
    long& x, long& y, long& width, long& height)
{
  // monitor
  RECT monitor_rect = {};
  if (!FetchMonitorRect(hwnd, &monitor_rect)) return;
  long monitor_width = static_cast<long>(monitor_rect.right - monitor_rect.left);
  long monitor_height = static_cast<long>(monitor_rect.bottom - monitor_rect.top);
  // window
  RECT client_rect = {};
  if (!GetClientRect(hwnd, &client_rect)) return;
  long client_width = static_cast<long>(client_rect.right - client_rect.left);
  long client_height = static_cast<long>(client_rect.bottom - client_rect.top);
  // target size
  CalcFullscreenSize(x, y, width, height,
      monitor_width, monitor_height, client_width, client_height);
}

static void CalcWindowFullscreenHalfSize(HWND hwnd,
    long& x, long& y, long& width, long& height)
{
  // monitor
  RECT monitor_rect = {};
  if (!FetchMonitorRect(hwnd, &monitor_rect)) return;
  long monitor_width = static_cast<long>(monitor_rect.right - monitor_rect.left) / 2;
  long monitor_height = static_cast<long>(monitor_rect.bottom - monitor_rect.top) / 2;
  // window
  RECT client_rect = {};
  if (!GetClientRect(hwnd, &client_rect)) return;
  long client_width = static_cast<long>(client_rect.right - client_rect.left);
  long client_height = static_cast<long>(client_rect.bottom - client_rect.top);
  // target size
  CalcFullscreenSize(x, y, width, height,
      monitor_width, monitor_height, client_width, client_height);
  x += monitor_width / 2;
  y += monitor_height / 2;
}

void ConfigureFullscreen(HWND hwnd) {
  long x = 0;
  long y = 0;
  long width = 0;
  long height = 0;
  CalcWindowFullscreenSize(hwnd, x, y, width, height);
  // set position and size
  SetWindowPos(hwnd, HWND_TOP,
      static_cast<int>(x), static_cast<int>(y),
      static_cast<int>(width), static_cast<int>(height),
      SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOSENDCHANGING);
}

void ConfigureDefaultWindow(HWND hwnd) {
  long x = 0;
  long y = 0;
  long width = 0;
  long height = 0;
  CalcWindowFullscreenHalfSize(hwnd, x, y, width, height);
  // set position and size
  SetWindowPos(hwnd, HWND_TOP,
      static_cast<int>(x), static_cast<int>(y),
      static_cast<int>(width), static_cast<int>(height),
      SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOSENDCHANGING);
}

}  // namespace imascs
