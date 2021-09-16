
#pragma once

#include "winrt_base.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>

#include <d3d11.h>
#include <dxgi1_2.h>

namespace imascs {
namespace capture {

winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface CreateDirect3DSurface(IDXGISurface* dxgi_surface);

}  // namespace capture
}  // namespace imascs
