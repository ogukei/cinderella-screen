
#include "device_d3d.h"

#include <d3d11.h>
#include <dxgi1_2.h>
#include <winrt/Windows.UI.Composition.h>
#include <windows.ui.composition.interop.h>

namespace imascs {
namespace capture {

static HRESULT CreateD3DDevice(const D3D_DRIVER_TYPE type, winrt::com_ptr<ID3D11Device>& device) {
  UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
  return D3D11CreateDevice(nullptr, type, nullptr, flags, nullptr, 0,
      D3D11_SDK_VERSION, device.put(), nullptr, nullptr);
}

winrt::com_ptr<ID3D11Device> CreateD3DDevice() {
  winrt::com_ptr<ID3D11Device> device;
  HRESULT hr = CreateD3DDevice(D3D_DRIVER_TYPE_HARDWARE, device);
  if (hr == DXGI_ERROR_UNSUPPORTED) {
    CreateD3DDevice(D3D_DRIVER_TYPE_WARP, device);
  }
  return device;
}

static winrt::com_ptr<IDXGISwapChain1> CreateDXGISwapChain(
    winrt::com_ptr<ID3D11Device> const& device,
    const DXGI_SWAP_CHAIN_DESC1* desc)
{
  auto dxgiDevice = device.as<IDXGIDevice2>();
  winrt::com_ptr<IDXGIAdapter> adapter;
  dxgiDevice->GetParent(winrt::guid_of<IDXGIAdapter>(), adapter.put_void());
  winrt::com_ptr<IDXGIFactory2> factory;
  adapter->GetParent(winrt::guid_of<IDXGIFactory2>(), factory.put_void());
  winrt::com_ptr<IDXGISwapChain1> swapchain;
  factory->CreateSwapChainForComposition(device.get(), desc, nullptr, swapchain.put());
  return swapchain;
}

winrt::com_ptr<IDXGISwapChain1> CreateDXGISwapChain(
    winrt::com_ptr<ID3D11Device> const& device,
    uint32_t width, uint32_t height,
    DXGI_FORMAT format, uint32_t bufferCount) 
{
  DXGI_SWAP_CHAIN_DESC1 desc = {};
  desc.Width = width;
  desc.Height = height;
  desc.Format = format;
  desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  desc.SampleDesc.Count = 1;
  desc.SampleDesc.Quality = 0;
  desc.BufferCount = bufferCount;
  desc.Scaling = DXGI_SCALING_STRETCH;
  desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
  desc.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
  return CreateDXGISwapChain(device, &desc);
}

}  // namespace capture
}  // namespace imascs
