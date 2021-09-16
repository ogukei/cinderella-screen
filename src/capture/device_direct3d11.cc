
#include "device_direct3d11.h"

#include <inspectable.h>

extern "C" {
  HRESULT __stdcall CreateDirect3D11DeviceFromDXGIDevice(
      ::IDXGIDevice* dxgiDevice,
      ::IInspectable** graphicsDevice);
}

namespace imascs {
namespace capture {

winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice CreateDirect3DDevice(IDXGIDevice* dxgi_device) {
  winrt::com_ptr<::IInspectable> d3d_device;
  winrt::check_hresult(CreateDirect3D11DeviceFromDXGIDevice(dxgi_device, d3d_device.put()));
  return d3d_device.as<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice>();
}

}  // namespace capture
}  // namespace imascs
