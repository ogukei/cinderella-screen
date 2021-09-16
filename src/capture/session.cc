
#include "session.h"

#include "device_d3d.h"
#include "device_direct3d11.h"

#include <iostream>

#include "context.h"
#include "capture_item.h"

#include "dxgi_interface.hpp"

namespace imascs {
namespace capture {

CaptureSession::CaptureSession() 
  : item_(nullptr),
    frame_pool_(nullptr),
    session_(nullptr)
{
}

void CaptureSession::Initialize(const std::unique_ptr<CaptureContext>& context, HWND hwnd) {
  auto item = CreateCaptureItemForWindow(hwnd);
  auto format = winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized;
  swap_chain_ = CreateDXGISwapChain(context->D3D11Device(), 
      static_cast<uint32_t>(item.Size().Width),
      static_cast<uint32_t>(item.Size().Height),
      static_cast<DXGI_FORMAT>(format),
      2);
  frame_pool_ = winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::Create(
      context->Direct3DDevice(),
      format,
      2,
      item.Size());
  item_ = item;
  session_ = frame_pool_.CreateCaptureSession(item_);
  frame_pool_.FrameArrived({ this, &CaptureSession::OnFrameArrived });
  device_context_ = context->D3D11DeviceContext();
  //
  session_.StartCapture();
}


void CaptureSession::OnFrameArrived(
    const winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool& sender,
    const winrt::Windows::Foundation::IInspectable& inspectable) 
{
  auto frame = sender.TryGetNextFrame();
  // copy
  winrt::com_ptr<ID3D11Texture2D> back_buffer;
  swap_chain_->GetBuffer(0, winrt::guid_of<ID3D11Texture2D>(), back_buffer.put_void());
  winrt::com_ptr<ID3D11Texture2D> texture = GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());
  device_context_->CopyResource(back_buffer.get(), texture.get());
  // present
  DXGI_PRESENT_PARAMETERS parameters{};
  swap_chain_->Present1(1, 0, &parameters);
}

}  // namespace capture
}  // namespace imascs
