
#include "image_encode.h"

#include "stream.h"

#include "dxgi_interface.hpp"

namespace imascs {
namespace capture {

ImageEncode::ImageEncode() : device_(nullptr) {

}

void ImageEncode::Initialize(
    const winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice& device)
{
  device_ = device;
  auto d3d_device = GetDXGIInterfaceFromObject<ID3D11Device>(device);
  d2d_factory_ = CreateD2DFactory();
  d2d_device_ = CreateD2DDevice(d2d_factory_, d3d_device);
  winrt::check_hresult(d2d_device_->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, d2d_context_.put()));
  imaging_factory_ = CreateWICFactory();
}

void ImageEncode::EncodeImagePNG(
    const winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface& surface,
    const winrt::Windows::Storage::Streams::IRandomAccessStream& stream)
{
  auto abi_stream = CreateStreamFromRandomAccessStream(stream);
  auto frame_texture = GetDXGIInterfaceFromObject<ID3D11Texture2D>(surface);
  D3D11_TEXTURE2D_DESC texture_desc = {};
  frame_texture->GetDesc(&texture_desc);
  auto dxgi_frame_texture = frame_texture.as<IDXGISurface>();
  //
  winrt::com_ptr<ID2D1Bitmap1> d2d_bitmap;
  winrt::check_hresult(
    d2d_context_->CreateBitmapFromDxgiSurface(dxgi_frame_texture.get(), nullptr, d2d_bitmap.put()));
  //
  auto dpi = 96.0f;
  WICImageParameters params = {};
  params.PixelFormat.format = texture_desc.Format;
  params.PixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
  params.DpiX = dpi;
  params.DpiY = dpi;
  params.PixelWidth = texture_desc.Width;
  params.PixelHeight = texture_desc.Height;
  //
  GUID imaging_container = GUID_ContainerFormatPng;
  WICPixelFormatGUID imaging_pixel_format = GUID_WICPixelFormat32bppBGRA;
  //
  winrt::com_ptr<IWICBitmapEncoder> encoder;
  winrt::check_hresult(imaging_factory_->CreateEncoder(imaging_container, nullptr, encoder.put()));
  winrt::check_hresult(encoder->Initialize(abi_stream.get(), WICBitmapEncoderNoCache));
  //
  winrt::com_ptr<IWICBitmapFrameEncode> imaging_frame;
  winrt::com_ptr<IPropertyBag2> frame_properties;
  winrt::check_hresult(encoder->CreateNewFrame(imaging_frame.put(), frame_properties.put()));
  winrt::check_hresult(imaging_frame->Initialize(frame_properties.get()));
  winrt::check_hresult(imaging_frame->SetPixelFormat(&imaging_pixel_format));
  //
  winrt::com_ptr<IWICImageEncoder> image_encoder;
  winrt::check_hresult(imaging_factory_->CreateImageEncoder(d2d_device_.get(), image_encoder.put()));
  winrt::check_hresult(image_encoder->WriteFrame(d2d_bitmap.get(), imaging_frame.get(), &params));
  winrt::check_hresult(imaging_frame->Commit());
  winrt::check_hresult(encoder->Commit());
}

}  // namespace capture
}  // namespace imascs
