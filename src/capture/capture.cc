
#include "capture.h"

#include <iostream>

#include "context.h"
#include "snapshot.h"
#include "image_encode.h"

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Storage.h>

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
  auto folder = co_await winrt::Windows::Storage::KnownFolders::GetFolderForUserAsync(nullptr, winrt::Windows::Storage::KnownFolderId::PicturesLibrary);
  auto file = co_await folder.CreateFileAsync(L"output.png",
    winrt::Windows::Storage::CreationCollisionOption::GenerateUniqueName);
  auto stream = co_await file.OpenAsync(winrt::Windows::Storage::FileAccessMode::ReadWrite);
  encode_->EncodeImagePNG(surface, stream);
  stream.Close();
}

}  // namespace capture
}  // namespace imascs
