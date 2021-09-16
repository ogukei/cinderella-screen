
#pragma once

#include "winrt_base.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <wincodec.h>

namespace imascs {
namespace capture {

winrt::com_ptr<IWICImagingFactory2> CreateWICFactory();

}  // namespace capture
}  // namespace imascs
