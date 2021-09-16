
#include "stream.h"

namespace imascs {
namespace capture {

winrt::com_ptr<IStream> CreateStreamFromRandomAccessStream(
    const winrt::Windows::Storage::Streams::IRandomAccessStream& stream)
{
  winrt::com_ptr<IStream> result;
  auto unknown = stream.as<::IUnknown>();
  winrt::check_hresult(::CreateStreamOverRandomAccessStream(unknown.get(), winrt::guid_of<IStream>(), result.put_void()));
  return result;
}

}  // namespace capture
}  // namespace imascs
