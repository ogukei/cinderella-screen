
#pragma once

#include <unknwn.h>
#include <winrt/base.h>

#include <d3d11.h>
#include <dxgi1_2.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.Graphics.Capture.h>

namespace imascs {
namespace capture {

class CaptureContext {
 private:
  winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice direct3DDevice_;
  winrt::com_ptr<ID3D11DeviceContext> d3d11DeviceContext_;
  winrt::com_ptr<ID3D11Device> d3d11Device_;

 public:
  CaptureContext();

  void Initialize();

  const winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice& Direct3DDevice() const { return direct3DDevice_; }
  const winrt::com_ptr<ID3D11DeviceContext>& D3D11DeviceContext() const { return d3d11DeviceContext_; }
  const winrt::com_ptr<ID3D11Device>& D3D11Device() const { return d3d11Device_; }
};

}  // namespace capture
}  // namespace imascs
