
#include "snapshot.h"

#include <iostream>
#include <dwmapi.h>

#include "device_d3d.h"
#include "device_direct3d11.h"
#include "surface_direct3d11.h"

#include "context.h"
#include "capture_item.h"

#include "completion_source.hpp"
#include "dxgi_interface.hpp"

namespace imascs {
namespace capture {

static bool CalcClientBox(HWND hwnd, D3D11_BOX* box) {
  RECT client_rect = {};
  if (!GetClientRect(hwnd, &client_rect)) return false;
  RECT window_rect = {};
  if (DwmGetWindowAttribute(hwnd,
    DWMWA_EXTENDED_FRAME_BOUNDS, &window_rect, sizeof(window_rect)) != S_OK) return false;
  POINT client_origin = {};
  if (!ClientToScreen(hwnd, &client_origin)) return false;
  long left = std::max(client_origin.x - window_rect.left, 0L);
  long top = std::max(client_origin.y - window_rect.top, 0L);
  long width = client_rect.right - client_rect.left;
  long height = client_rect.bottom - client_rect.top;
  box->left = left;
  box->top = top;
  box->right = left + width;
  box->bottom = top + height;
  box->front = 0;
  box->back = 1;
  return true;
}

static std::optional<D3D11_BOX> WindowSubregionBox(
    HWND hwnd,
    const winrt::Windows::Graphics::SizeInt32& size)
{
  D3D11_BOX box = {};
  if (!CalcClientBox(hwnd, &box)) return std::nullopt;
  long width = box.right - box.left;
  long height = box.bottom - box.top;
  bool is_entire = (width == size.Width && height == size.Height);
  if (is_entire) return std::nullopt;
  return box;
}

CaptureSnapshot::CaptureSnapshot() {
}

winrt::Windows::Foundation::IAsyncOperation<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface>
CaptureSnapshot::TakeAsync(const std::unique_ptr<CaptureContext>& context, HWND hwnd) {
  auto item = CreateCaptureItemForWindow(hwnd);
  auto size = item.Size();
  auto format = winrt::Windows::Graphics::DirectX::DirectXPixelFormat::B8G8R8A8UIntNormalized;
  auto frame_pool = winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::CreateFreeThreaded(
      context->Direct3DDevice(),
      format,
      1,
      size);
  auto session = frame_pool.CreateCaptureSession(item);
  session.IsCursorCaptureEnabled(false);
  auto subregion = WindowSubregionBox(hwnd, size);
  auto completion = completion_source<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface>();
  auto& device_d3d11 = context->D3D11Device();
  auto& device_context = context->D3D11DeviceContext();
  frame_pool.FrameArrived([device_d3d11, device_context, session, subregion, &completion](auto& frame_pool, auto&) {
    auto frame = frame_pool.TryGetNextFrame();
    auto frame_texture = GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());
    // copy
    D3D11_TEXTURE2D_DESC desc = {};
    frame_texture->GetDesc(&desc);
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    // adjust size to cropping destination
    if (subregion.has_value()) {
      auto& box = subregion.value();
      desc.Width = box.right - box.left;
      desc.Height = box.bottom - box.top;
    }
    winrt::com_ptr<ID3D11Texture2D> texture_copy;
    winrt::check_hresult(device_d3d11->CreateTexture2D(&desc, nullptr, texture_copy.put()));
    // client rect cropping
    if (subregion.has_value()) {
      auto& box = subregion.value();
      device_context->CopySubresourceRegion(texture_copy.get(), 0, 0, 0, 0, frame_texture.get(), 0, &box);
    } else {
      // entire window
      device_context->CopyResource(texture_copy.get(), frame_texture.get());
    }
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
  return co_await completion;
}

}  // namespace capture
}  // namespace imascs
