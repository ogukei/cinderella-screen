
#include "capture.h"

#include <iostream>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.System.h>

#include "context.h"
#include "snapshot.h"
#include "image_encode.h"

#include "capture_utility.hpp"

namespace imascs {
namespace capture {

CaptureFacade::CaptureFacade(const imascs::core::WinRTContext& winrt_context) {
  auto context = std::make_unique<CaptureContext>(winrt_context);
  context->Initialize();
  auto encode = std::make_unique<ImageEncode>();
  encode->Initialize(context->Direct3DDevice());
  context_ = std::move(context);
  encode_ = std::move(encode);
}

CaptureFacade& CaptureFacade::Instance(const imascs::core::WinRTContext& context) {
  static CaptureFacade instance(context);
  return instance;
}

winrt::fire_and_forget CaptureFacade::SnapshotWindow(HWND hwnd) {
  if (hwnd == NULL) return;
  auto snapshot = std::make_unique<CaptureSnapshot>();
  auto surface = co_await snapshot->TakeAsync(context_, hwnd);
  auto folder = co_await winrt::Windows::Storage::KnownFolders::GetFolderForUserAsync(nullptr, winrt::Windows::Storage::KnownFolderId::Screenshots);
  std::string filename = "Screenshot_" + FilenameSafeDatetimeString() + ".png";
  auto file = co_await folder.CreateFileAsync(winrt::to_hstring(filename),
    winrt::Windows::Storage::CreationCollisionOption::GenerateUniqueName);
  auto stream = co_await file.OpenAsync(winrt::Windows::Storage::FileAccessMode::ReadWrite);
  encode_->EncodeImagePNG(surface, stream);
  stream.Close();
}

winrt::fire_and_forget CaptureFacade::LaunchScreenshotFolder() {
  auto folder = co_await winrt::Windows::Storage::KnownFolders::GetFolderForUserAsync(nullptr, winrt::Windows::Storage::KnownFolderId::Screenshots);
  co_await winrt::Windows::System::Launcher::LaunchFolderAsync(folder);
}

}  // namespace capture
}  // namespace imascs
