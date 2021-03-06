
#pragma once

#include "winrt_base.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <d3d11.h>
#include <dxgi1_2.h>

namespace imascs {
namespace capture {

winrt::com_ptr<ID3D11Device> CreateD3DDevice();
winrt::com_ptr<IDXGISwapChain1> CreateDXGISwapChain(
    winrt::com_ptr<ID3D11Device> const& device,
    uint32_t width, uint32_t height,
    DXGI_FORMAT format, uint32_t bufferCount);

}  // namespace capture
}  // namespace imascs
