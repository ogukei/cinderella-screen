
#include "context.h"

#include "device_d3d.h"
#include "device_direct3d11.h"

namespace imascs {
namespace capture {

CaptureContext::CaptureContext(const imascs::core::WinRTContext& winrt_context) {
  // having winrt_context, make sure we have winrt::init_apartment() done
}

void CaptureContext::Initialize() {
  // ID3D11Device
  auto d3d11_device = CreateD3DDevice();
  auto dxgi_device = d3d11_device.as<IDXGIDevice>();
  // winrt IDirect3DDevice
  device_ = CreateDirect3DDevice(dxgi_device.get());
  // ID3D11DeviceContext
  d3d11_device->GetImmediateContext(d3d11_device_context_.put());
  d3d11_device_ = d3d11_device;
}

}  // namespace capture
}  // namespace imascs
