
#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

#include <memory>

#include "winrt_base.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Capture.h>

namespace imascs {
namespace core {

class WinRTContext;

}  // namespace core
}  // namespace imascs

namespace imascs {
namespace capture {

class CaptureContext;
class CaptureSession;

class CaptureFacade {
 private:
  std::unique_ptr<CaptureContext> context_;
  CaptureFacade(const imascs::core::WinRTContext& context);

 public:
  CaptureFacade(const CaptureFacade&) = delete;

  static CaptureFacade& Instance(const imascs::core::WinRTContext& context);

  void SnapshotWindow(HWND hwnd);
};

}  // namespace capture
}  // namespace imascs
