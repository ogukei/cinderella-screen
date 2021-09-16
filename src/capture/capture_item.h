
#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

#include "winrt_base.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Capture.h>

namespace imascs {
namespace capture {

winrt::Windows::Graphics::Capture::GraphicsCaptureItem CreateCaptureItemForWindow(HWND hwnd);

}  // namespace capture
}  // namespace imascs
