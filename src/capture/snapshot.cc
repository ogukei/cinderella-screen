
#include "snapshot.h"

#include "device_d3d.h"
#include "device_direct3d11.h"
#include "surface_direct3d11.h"

#include <iostream>

#include "context.h"
#include "capture_item.h"

#include "surface_direct3d11.hpp"
#include "completion_source.hpp"

namespace imascs {
namespace capture {

CaptureSnapshot::CaptureSnapshot() {
}

winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface CaptureSnapshot::Take(const std::unique_ptr<CaptureContext>& context, HWND hwnd) {
  auto item = CreateCaptureItemForWindow(hwnd);
  auto format = winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized;
  auto frame_pool = winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::CreateFreeThreaded(
      context->Direct3DDevice(),
      format,
      1,
      item.Size());
  auto session = frame_pool.CreateCaptureSession(item);
  // 
  auto completion = completion_source<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface>();
  auto& device_d3d11 = context->D3D11Device();
  auto& device_context = context->D3D11DeviceContext();
  frame_pool.FrameArrived([device_d3d11, device_context, session, &completion](auto& frame_pool, auto&) {
    auto frame = frame_pool.TryGetNextFrame();
    auto frame_texture = GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());
    // copy
    D3D11_TEXTURE2D_DESC desc = {};
    frame_texture->GetDesc(&desc);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    winrt::com_ptr<ID3D11Texture2D> texture_copy;
    winrt::check_hresult(device_d3d11->CreateTexture2D(&desc, nullptr, texture_copy.put()));
    device_context->CopyResource(texture_copy.get(), frame_texture.get());
    // result
    auto dxgi_surface = texture_copy.as<IDXGISurface>();
    auto result = CreateDirect3DSurface(dxgi_surface.get());
    // clean up
    session.Close();
    frame_pool.Close();
    // finish
    completion.set(result);
  });
  session.StartCapture();
  // 
  completion.await_ready();
  return completion.await_resume();
}

}  // namespace capture
}  // namespace imascs
