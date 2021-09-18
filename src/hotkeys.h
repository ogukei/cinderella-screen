
#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#define NOMINMAX
#include <windows.h>

namespace imascs {

LRESULT CALLBACK HotkeysHookProc(int nCode, WPARAM wParam, LPARAM lParam);

void ConfigureGlobalHotkeys();

}  // namespace imascs
