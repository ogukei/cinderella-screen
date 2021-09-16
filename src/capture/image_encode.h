
#pragma once

#include <memory>

#include "winrt_base.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Capture.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.Storage.Streams.h>

#include "device_d2d.h"
#include "device_direct3d11.h"
#include "device_codec.h"

namespace imascs {
namespace capture {

class ImageEncode {
 private:
  winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice device_;
  winrt::com_ptr<ID2D1Factory1> d2d_factory_;
  winrt::com_ptr<ID2D1Device> d2d_device_;
  winrt::com_ptr<ID2D1DeviceContext> d2d_context_;
  winrt::com_ptr<IWICImagingFactory2> imaging_factory_;
 public:
  ImageEncode();
  ImageEncode(const ImageEncode&) = delete;

  void Initialize(
    const winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice& device);
  void EncodeImagePNG(
    const winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface& surface,
    const winrt::Windows::Storage::Streams::IRandomAccessStream& stream);
};

}  // namespace capture
}  // namespace imascs
