
#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

namespace imascs {

void MakeTargetAppFullscreen();
void MakeTargetAppForeground();
void MakeTargetAppBackground();
void MakeTargetAppDefault();

void SnapshotTargetAppWindow();

}  // namespace imascs
