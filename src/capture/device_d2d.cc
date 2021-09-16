
#include "device_d2d.h"

#include <dxgi1_2.h>

namespace imascs {
namespace capture {

winrt::com_ptr<ID2D1Factory1> CreateD2DFactory() {
  D2D1_FACTORY_OPTIONS options{};
  winrt::com_ptr<ID2D1Factory1> factory;
  winrt::check_hresult(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, factory.put()));
  return factory;
}

winrt::com_ptr<ID2D1Device> CreateD2DDevice(
    const winrt::com_ptr<ID2D1Factory1>& factory,
    const winrt::com_ptr<ID3D11Device>& device)
{
  winrt::com_ptr<ID2D1Device> result;
  winrt::check_hresult(factory->CreateDevice(device.as<IDXGIDevice>().get(), result.put()));
  return result;
}

}  // namespace capture
}  // namespace imascs
