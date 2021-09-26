
#include "fullscreen.h"

#include <iostream>
#include <numeric>
#include <algorithm>

#include "window_helper.h"

namespace imascs {

void ResizeWindow(HWND hwnd, const WindowResizeConfiguration& configuration) {
  long x = configuration.Offset().x;
  long y = configuration.Offset().y;
  long width = configuration.Size().cx;
  long height = configuration.Size().cy;
  // set position and size
  SetWindowPos(hwnd, HWND_TOP,
      static_cast<int>(x), static_cast<int>(y),
      static_cast<int>(width), static_cast<int>(height),
      SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREPOSITION | SWP_NOSENDCHANGING);
}

void ConfigureFullscreen(HWND hwnd) {
  WindowResizeConstraint constraint(hwnd);
  WindowResizeConfiguration configuration(constraint);
  ResizeWindow(hwnd, configuration);
}

void ConfigureDefaultWindow(HWND hwnd) {
  WindowResizeConstraint constraint(hwnd);
  constraint.SetHorizontalAlignment(WindowHorizontalAlignment::center);
  constraint.SetResizeMode(WindowResizeMode::three_quaters_of_screen);
  WindowResizeConfiguration configuration(constraint);
  ResizeWindow(hwnd, configuration);
}

static std::optional<SIZE> MonitorSize(HMONITOR monitor) {
  RECT monitor_rect = {};
  if (!FetchMonitorRect(monitor, &monitor_rect)) return std::nullopt;
  long monitor_width = static_cast<long>(monitor_rect.right - monitor_rect.left);
  long monitor_height = static_cast<long>(monitor_rect.bottom - monitor_rect.top);
  SIZE size = {monitor_width, monitor_height};
  return size;
}

static std::optional<SIZE> WindowClientSize(HWND hwnd) {
  RECT client_rect = {};
  if (!GetClientRect(hwnd, &client_rect)) return std::nullopt;
  long client_width = static_cast<long>(client_rect.right - client_rect.left);
  long client_height = static_cast<long>(client_rect.bottom - client_rect.top);
  SIZE size = {client_width, client_height};
  return size;
}

static HMONITOR InitialWindowMonitor(HWND hwnd) {
  return MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
}

WindowResizeConstraint::WindowResizeConstraint(HWND hwnd)
  : WindowResizeConstraint(InitialWindowMonitor(hwnd), hwnd) {
}

static SIZE InitialAspect(HWND hwnd) {
  SIZE client_size = WindowClientSize(hwnd).value();
  bool is_landscape = client_size.cx >= client_size.cy;
  SIZE aspect_ratio = (is_landscape) ? SIZE{16, 9} : SIZE{9, 16};
  return aspect_ratio;
}

static SIZE InitialFrameSize(HMONITOR monitor) {
  SIZE monitor_size = MonitorSize(monitor).value();
  return monitor_size;
}

WindowResizeConstraint::WindowResizeConstraint(HMONITOR monitor, HWND hwnd)
  : WindowResizeConstraint(InitialFrameSize(monitor), InitialAspect(hwnd)) {
}

WindowResizeConstraint::WindowResizeConstraint(const SIZE& frame_size, const SIZE& aspect_ratio) {
  desired_size_ = frame_size;
  frame_size_ = frame_size;
  aspect_ratio_ = aspect_ratio;
  resize_mode_ = WindowResizeMode::fullscreen;
  alignment_ = WindowHorizontalAlignment::center;
}

static SIZE AdjustSizeToAspect(const SIZE& size, const SIZE& aspect) {
  double ax = static_cast<double>(aspect.cx);
  double ay = static_cast<double>(aspect.cy);
  double w = static_cast<double>(size.cx);
  double h = static_cast<double>(size.cy);
  long r = 0;
  if ((w / h) < (ax / ay)) {
    r = size.cx / aspect.cx;
    if ((size.cx % aspect.cx) != 0) {
      r += 1;
    }
  } else {
    r = size.cy / aspect.cy;
    if ((size.cy % aspect.cy) != 0) {
      r += 1;
    }
  }
  w = static_cast<double>(r) * ax;
  h = static_cast<double>(r) * ay; 
  long width = std::lround(w);
  long height = std::lround(h);
  return {width, height};
}

static bool IsEqualAspect(const SIZE& size, const SIZE& aspect) {
  long d = std::gcd(size.cx, size.cy);
  long rw = size.cx / d;
  long rh = size.cy / d;
  return aspect.cx == rw && aspect.cy == rh;
}

static POINT AdjustOffset(WindowHorizontalAlignment alignment,
    const SIZE& target_size, const SIZE& frame_size) 
{
  long x = 0;
  long y = 0;
  switch (alignment) {
  case WindowHorizontalAlignment::left:
    x = 0;
    y = (frame_size.cy - target_size.cy) / 2;
    break;
  case WindowHorizontalAlignment::right:
    x = frame_size.cx - target_size.cx;
    y = (frame_size.cy - target_size.cy) / 2;
    break;
  case WindowHorizontalAlignment::center:
  default:
    x = (frame_size.cx - target_size.cx) / 2;
    y = (frame_size.cy - target_size.cy) / 2;
    break;
  }
  return {x, y};
}

static SIZE AdjustSizeByScale(const SIZE& size, WindowResizeMode resize_mode) {
  switch (resize_mode) {
  case WindowResizeMode::fullscreen:
    return size;
  case WindowResizeMode::three_quaters_of_screen:
    return {size.cx * 3 / 4, size.cy * 3 / 4};
  }
  return size;
}

WindowResizeConfiguration::WindowResizeConfiguration(const WindowResizeConstraint& constraint) {
  SIZE size = constraint.DesiredSize();
  size = AdjustSizeByScale(size, constraint.ResizeMode());
  if (!IsEqualAspect(size, constraint.AspectRatio())) {
    size = AdjustSizeToAspect(size, constraint.AspectRatio());
  }
  offset_ = AdjustOffset(constraint.HorizontalAlignment(), size, constraint.FrameSize());
  size_ = size;
}

}  // namespace imascs
