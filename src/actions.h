
#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

namespace imascs {

void MakeTargetAppFullscreen();
void MakeTargetAppForeground();
void MakeTargetAppBackground();
void MakeTargetAppPictureInPicture();
void MakeTargetAppDefault();

void SnapshotTargetAppWindow();
void LaunchScreenshotFolder();

}  // namespace imascs
