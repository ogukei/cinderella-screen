
#pragma once

#include <memory>

#include "winrt_base.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>

#include <d3d11.h>
#include <dxgi1_2.h>

namespace imascs {
namespace capture {

class CaptureContext;

class CaptureSession {
 private:
  winrt::com_ptr<IDXGISwapChain1> swap_chain_;
  winrt::com_ptr<ID3D11DeviceContext> device_context_;
  winrt::Windows::Graphics::Capture::GraphicsCaptureItem item_;
  winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool frame_pool_;
  winrt::Windows::Graphics::Capture::GraphicsCaptureSession session_;

 public:
  CaptureSession();

  void Initialize(const std::unique_ptr<CaptureContext>& context, HWND hwnd);
  void OnFrameArrived(
     const winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool& sender,
     const winrt::Windows::Foundation::IInspectable&);
};

}  // namespace capture
}  // namespace imascs
