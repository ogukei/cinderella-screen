
#pragma once

#include <unknwn.h>
#include <winrt/base.h>

#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <d3d11.h>
#include <dxgi1_2.h>

namespace imascs {
namespace capture {

winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice CreateDirect3DDevice(IDXGIDevice* dxgi_device);

}  // namespace capture
}  // namespace imascs
