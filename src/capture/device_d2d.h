
#pragma once

#include "winrt_base.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <d2d1_1.h>
#include <d3d11.h>

namespace imascs {
namespace capture {

winrt::com_ptr<ID2D1Factory1> CreateD2DFactory();
winrt::com_ptr<ID2D1Device> CreateD2DDevice(
    const winrt::com_ptr<ID2D1Factory1>& factory,
    const winrt::com_ptr<ID3D11Device>& device);

}  // namespace capture
}  // namespace imascs
