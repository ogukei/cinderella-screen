
#pragma once

#include <functional>

namespace imascs {
namespace core {

class WinRTContext {
 private:
  WinRTContext();

 public:
  WinRTContext(const WinRTContext&) = delete;

  static WinRTContext& Instance();

#ifdef _DEBUG
  void RunDebug(const std::function<void(void)> &func);
#endif
};

}  // namespace core
}  // namespace imascs
