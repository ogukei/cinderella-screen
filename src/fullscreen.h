
#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#define NOMINMAX
#include <windows.h>

namespace imascs {

void ConfigureFullscreen(HWND hwnd);
void ConfigureDefaultWindow(HWND hwnd);

}  // namespace imascs
