
#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#define NOMINMAX
#include <windows.h>

#include <optional>
#include <array>
#include <memory>
#include <cstdint>

namespace imascs {

enum class WindowResizeMode {
  fullscreen,
  three_quaters_of_screen
};

enum class WindowHorizontalAlignment {
  center,
  left,
  right
};

class WindowResizeConstraint {
 private:
  WindowResizeMode resize_mode_;
  SIZE desired_size_;
  SIZE aspect_ratio_;
  SIZE frame_size_;
  WindowHorizontalAlignment alignment_;

 public:
  explicit WindowResizeConstraint(HWND hwnd);
  explicit WindowResizeConstraint(HMONITOR monitor, HWND hwnd);

  void SetHorizontalAlignment(WindowHorizontalAlignment alignment) { alignment_ = alignment; }
  void SetResizeMode(WindowResizeMode resize_mode) { resize_mode_ = resize_mode; }

  const SIZE& DesiredSize() const { return desired_size_; }
  const SIZE& AspectRatio() const { return aspect_ratio_; }
  const SIZE& FrameSize() const { return frame_size_; }
  WindowHorizontalAlignment HorizontalAlignment() const { return alignment_; }
  WindowResizeMode ResizeMode() const { return resize_mode_; }
};

class WindowResizeConfiguration {
 private:
  POINT offset_;
  SIZE size_;
 public:
  explicit WindowResizeConfiguration(const WindowResizeConstraint& constraint);

  const POINT& Offset() const { return offset_; }
  const SIZE& Size() const { return size_; }
};

void ResizeWindow(HWND hwnd, const WindowResizeConfiguration& configuration);
void ConfigureFullscreen(HWND hwnd);
void ConfigureDefaultWindow(HWND hwnd);

}  // namespace imascs
