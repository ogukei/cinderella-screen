
#pragma once

#include "winrt_base.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>

#include <d3d11.h>
#include <dxgi1_2.h>

struct __declspec(uuid("A9B3D012-3DF2-4EE3-B8D1-8695F457D3C1"))
    IDirect3DDxgiInterfaceAccess : ::IUnknown {
  virtual HRESULT __stdcall GetInterface(const GUID& id, void** object) = 0;
};

namespace imascs {
namespace capture {

template <typename T>
winrt::com_ptr<T> GetDXGIInterfaceFromObject(const winrt::Windows::Foundation::IInspectable& object) {
  auto access = object.as<IDirect3DDxgiInterfaceAccess>();
  winrt::com_ptr<T> result;
  winrt::check_hresult(access->GetInterface(winrt::guid_of<T>(), result.put_void()));
  return result;
}

}  // namespace capture
}  // namespace imascs
