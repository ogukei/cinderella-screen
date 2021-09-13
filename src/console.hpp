
#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <iostream>

struct ConsoleActivationGuard {
  ConsoleActivationGuard() {
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
  }
  ConsoleActivationGuard(const ConsoleActivationGuard&) = delete;

  ~ConsoleActivationGuard() {
    Sleep(1000);
    FreeConsole();
  }
};
