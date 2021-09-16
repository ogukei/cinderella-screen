

#include "ui.h"

#include <iostream>

#include "resource.h"
#include "window_helper.h"

#define UI_BUTTON_WIDTH 250
#define UI_BUTTON_HEIGHT 50
#define UI_BUTTON_MARGIN_X 10
#define UI_BUTTON_MARGIN_Y 10
#define UI_WINDOW_MARGIN_X 140
#define UI_COLUMN_WIDTH 70
#define UI_VERTICAL_SPACER 24

namespace imascs {

SIZE GetUIBoundingBox() {
  int cx = UI_BUTTON_MARGIN_X * 2 + UI_BUTTON_WIDTH;
  cx += UI_WINDOW_MARGIN_X;
  int cy = UI_BUTTON_MARGIN_Y * 5 + UI_BUTTON_HEIGHT * 4;
  cy += UI_VERTICAL_SPACER;
  RECT rect = {0, 0, cx, cy};
  AdjustWindowRectEx(&rect, GetOverlappedWindowStyle(), true, 0);
  SIZE size = {rect.right - rect.left, rect.bottom - rect.top};
  return size;
}

void SetupUI(HINSTANCE hInstance, HWND hwnd) {
  int x = UI_BUTTON_MARGIN_X + UI_COLUMN_WIDTH;
  int y = UI_BUTTON_MARGIN_Y;
  //
  MakeButton(hInstance, hwnd, x, y, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT,
      L"Fullscreen (F11)", (HMENU)IDM_ACTION1);
  y += UI_BUTTON_HEIGHT + UI_BUTTON_MARGIN_Y;
  //
  MakeButton(hInstance, hwnd, x, y, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT,
      L"Desktop Wallpaper (F10)", (HMENU)IDM_ACTION2);
  y += UI_BUTTON_HEIGHT + UI_BUTTON_MARGIN_Y;
  //
  MakeButton(hInstance, hwnd, x, y, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT,
      L"Default (F9)", (HMENU)IDM_ACTION3);
  y += UI_BUTTON_HEIGHT + UI_BUTTON_MARGIN_Y;
  y += UI_VERTICAL_SPACER;
  // 
  MakeButton(hInstance, hwnd, x, y, UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT,
      L"Screenshot (F12)", (HMENU)IDM_ACTION4);
}

}  // namespace imascs
