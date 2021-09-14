
#include "context.h"

#include "device_d3d.h"
#include "device_direct3d11.h"

namespace imascs {
namespace capture {

CaptureContext::CaptureContext() {
}

void CaptureContext::Initialize() {
  // ID3D11Device
  auto d3dDevice = CreateD3DDevice();
  auto dxgiDevice = d3dDevice.as<IDXGIDevice>();
  // winrt IDirect3DDevice
  direct3DDevice_ = CreateDirect3DDevice(dxgiDevice.get());
  // ID3D11DeviceContext
  d3dDevice->GetImmediateContext(d3d11DeviceContext_.put());
  d3d11Device_ = d3dDevice;
}

}  // namespace capture
}  // namespace imascs
