
#include "session.h"

#include "device_d3d.h"
#include "device_direct3d11.h"

#include <iostream>

#include "context.h"
#include "capture_item.h"

namespace imascs {
namespace capture {

CaptureSession::CaptureSession() 
  : item_(nullptr),
    framePool_(nullptr)
{
}

void CaptureSession::Initialize(const std::unique_ptr<CaptureContext>& context, HWND hwnd) {
  auto item = CreateCaptureItemForWindow(hwnd);
  auto format = winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized;
  dxgiSwapChain_ = CreateDXGISwapChain(context->D3D11Device(), 
      static_cast<uint32_t>(item.Size().Width),
      static_cast<uint32_t>(item.Size().Height),
      static_cast<DXGI_FORMAT>(format),
      2);
  framePool_ = winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::Create(
      context->Direct3DDevice(),
      format,
      2,
      item.Size());
  item_ = item;
}

}  // namespace capture
}  // namespace imascs
