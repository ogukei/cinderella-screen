
#pragma once

#include <unknwn.h>
#include <winrt/Windows.Graphics.Capture.h>

#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

namespace imascs {
namespace capture {

winrt::Windows::Graphics::Capture::GraphicsCaptureItem CreateCaptureItemForWindow(HWND hwnd);

}  // namespace capture
}  // namespace imascs
