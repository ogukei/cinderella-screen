
#include "capture.h"

#include <iostream>

#include "context.h"
#include "session.h"

namespace imascs {
namespace capture {

void CaptureFoo(const imascs::core::WinRTContext& winrt_context, HWND hwnd) {
  if (hwnd == NULL) return; 
  auto context = std::make_unique<CaptureContext>();
  context->Initialize();
  auto session = std::make_unique<CaptureSession>();
  session->Initialize(context, hwnd);
}

}  // namespace capture
}  // namespace imascs
