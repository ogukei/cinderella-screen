
#include "device_codec.h"

namespace imascs {
namespace capture {

winrt::com_ptr<IWICImagingFactory2> CreateWICFactory() {
  return winrt::create_instance<IWICImagingFactory2>(CLSID_WICImagingFactory2);
}

}  // namespace capture
}  // namespace imascs
