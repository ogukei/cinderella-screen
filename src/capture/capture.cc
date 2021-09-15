
#include "capture.h"

#include <iostream>

#include "context.h"
#include "snapshot.h"

namespace imascs {
namespace capture {

CaptureFacade::CaptureFacade(const imascs::core::WinRTContext& winrt_context)
  : context_(std::make_unique<CaptureContext>(winrt_context)) 
{
  context_->Initialize();
}

CaptureFacade& CaptureFacade::Instance(const imascs::core::WinRTContext& context) {
  static CaptureFacade instance(context);
  return instance;
}

void CaptureFacade::SnapshotWindow(HWND hwnd) {
  if (hwnd == NULL) return; 
  auto snapshot = std::make_unique<CaptureSnapshot>();
  auto surface = snapshot->Take(context_, hwnd);
  std::cout << "OK" << std::endl;
}

}  // namespace capture
}  // namespace imascs
