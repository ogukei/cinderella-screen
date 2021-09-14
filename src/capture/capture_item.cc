
#include "capture_item.h"

#include <windows.graphics.capture.interop.h>
#include <windows.graphics.capture.h>

namespace imascs {
namespace capture {

winrt::Windows::Graphics::Capture::GraphicsCaptureItem CreateCaptureItemForWindow(HWND hwnd) {
		auto interop_factory = winrt::get_activation_factory<winrt::Windows::Graphics::Capture::GraphicsCaptureItem, IGraphicsCaptureItemInterop>();
		winrt::Windows::Graphics::Capture::GraphicsCaptureItem item = { nullptr };
		winrt::check_hresult(interop_factory->CreateForWindow(hwnd,
				winrt::guid_of<ABI::Windows::Graphics::Capture::IGraphicsCaptureItem>(),
				winrt::put_abi(item)));
		return item;
}

}  // namespace capture
}  // namespace imascs
