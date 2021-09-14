
#pragma once

#include <memory>

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

class CaptureContext;

class CaptureSession {
 private:
  winrt::com_ptr<IDXGISwapChain1> dxgiSwapChain_;
  winrt::Windows::Graphics::Capture::GraphicsCaptureItem item_;
  winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool framePool_;
 public:
  CaptureSession();

  void Initialize(const std::unique_ptr<CaptureContext>& context, HWND hwnd);
};

}  // namespace capture
}  // namespace imascs
