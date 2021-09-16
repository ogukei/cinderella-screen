
#include "winrt_context.h"

#include <iostream>

#include <winrt/Windows.Foundation.h>

namespace imascs {
namespace core {

WinRTContext::WinRTContext() {
  winrt::init_apartment();
}

WinRTContext& WinRTContext::Instance() {
  static WinRTContext instance;
  return instance;
}

#ifdef _DEBUG
void WinRTContext::RunDebug(const std::function<void(void)>& func) {
  try {
    func();
  } catch (const winrt::hresult_error& ex) {
    winrt::hresult hr = ex.code();
    winrt::hstring message = ex.message();
    std::wcout << message.c_str() << std::endl;
  }
}
#endif

}  // namespace core
}  // namespace imascs
