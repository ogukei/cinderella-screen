
#pragma once

#include "winrt_base.hpp"
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Storage.Streams.h>

#include <shcore.h>

namespace imascs {
namespace capture {

winrt::com_ptr<IStream> CreateStreamFromRandomAccessStream(
  const winrt::Windows::Storage::Streams::IRandomAccessStream& stream);

}  // namespace capture
}  // namespace imascs
