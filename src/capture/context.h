
#pragma once

#include "winrt_base.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>

#include <d3d11.h>
#include <dxgi1_2.h>

namespace imascs {
namespace core {

class WinRTContext;

}  // namespace core
}  // namespace imascs

namespace imascs {
namespace capture {

class CaptureContext {
 private:
  winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice device_;
  winrt::com_ptr<ID3D11DeviceContext> d3d11_device_context_;
  winrt::com_ptr<ID3D11Device> d3d11_device_;

 public:
  explicit CaptureContext(const imascs::core::WinRTContext& winrt_context);
  CaptureContext(const CaptureContext&) = delete;

  void Initialize();

  const winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice& Direct3DDevice() const { return device_; }
  const winrt::com_ptr<ID3D11DeviceContext>& D3D11DeviceContext() const { return d3d11_device_context_; }
  const winrt::com_ptr<ID3D11Device>& D3D11Device() const { return d3d11_device_; }
};

}  // namespace capture
}  // namespace imascs
