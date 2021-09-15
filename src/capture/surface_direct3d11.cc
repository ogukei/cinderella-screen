
#include "surface_direct3d11.h"

#include <inspectable.h>

extern "C" {
  HRESULT __stdcall CreateDirect3D11SurfaceFromDXGISurface(::IDXGISurface* dgxiSurface,
      ::IInspectable** graphicsSurface);
}

namespace imascs {
namespace capture {

winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface CreateDirect3DSurface(IDXGISurface* dxgi_surface) {
  winrt::com_ptr<::IInspectable> d3d_surface;
  winrt::check_hresult(CreateDirect3D11SurfaceFromDXGISurface(dxgi_surface, d3d_surface.put()));
  return d3d_surface.as<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface>();
}

}  // namespace capture
}  // namespace imascs

