
#pragma once

#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>

namespace imascs {

void SetupUI(HINSTANCE hInstance, HWND hwnd);
SIZE GetUIBoundingBox();

}  // namespace imascs
