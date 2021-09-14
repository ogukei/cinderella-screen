
#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

namespace imascs {
namespace core {

class WinRTContext;

}  // namespace core
}  // namespace imascs

namespace imascs {
namespace capture {

void CaptureFoo(const imascs::core::WinRTContext& context, HWND hwnd);

}  // namespace capture
}  // namespace imascs
