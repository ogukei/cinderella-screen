
#include "window_helper.h"

#include <iostream>

#include <tlhelp32.h>

namespace imascs {

DWORD FindProcessIDByName(LPCWSTR name) {
  DWORD pid = 0;
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  PROCESSENTRY32 process = {sizeof(process)};
  BOOL continues = Process32First(snapshot, &process);
  while (continues) {
    if (wcscmp(process.szExeFile, name) == 0) {
      pid = process.th32ProcessID;
      break;
    }
    continues = Process32Next(snapshot, &process);
  }
  CloseHandle(snapshot);
  return pid;
}

HWND FindMainWindow(DWORD pid) {
  HWND hwnd = NULL;
  for (;;) {
    hwnd = FindWindowEx(NULL, hwnd, NULL, NULL);
    if (hwnd == NULL) break;
    DWORD dwProcessId = 0;
    GetWindowThreadProcessId(hwnd, &dwProcessId);
    if (dwProcessId == pid) {
      if (GetWindow(hwnd, GW_OWNER) == NULL && IsWindowVisible(hwnd)) {
        return hwnd;
      }
    }
  }
  return NULL;
}

void ConfigureBorderless(HWND hwnd) {
  LONG style = GetWindowLong(hwnd, GWL_STYLE);
  LONG exStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
  style &= ~(WS_CAPTION | WS_THICKFRAME);
  exStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
  SetWindowLong(hwnd, GWL_STYLE, style);
  SetWindowLong(hwnd, GWL_EXSTYLE, exStyle);
}

void ConfigureDefaultBorder(HWND hwnd) {
  LONG style = GetWindowLong(hwnd, GWL_STYLE);
  style |= (WS_CAPTION | WS_THICKFRAME);
  SetWindowLong(hwnd, GWL_STYLE, style);
}

bool FetchMonitorRect(HWND hwnd, RECT* rectOut) {
  HMONITOR monitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  MONITORINFO info = {sizeof(info)};
  if (GetMonitorInfo(monitor, &info)) {
    *rectOut = info.rcMonitor;
    return true;
  }
  return false;
}

BOOL EnablePrivilege(LPCWSTR lpPrivilegeName) {
  HANDLE token = NULL;
  OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token);
  if (token == NULL) {
    return false;
  }
  BOOL bResult = FALSE;
  {
    LUID luid;
    if (LookupPrivilegeValue(NULL, lpPrivilegeName, &luid)) {
      TOKEN_PRIVILEGES priv = {};
      priv.PrivilegeCount = 1;
      priv.Privileges[0].Luid = luid;
      priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      bResult = AdjustTokenPrivileges(token, FALSE, &priv, sizeof(priv), NULL, NULL);
    }
  }
  CloseHandle(token);
  return bResult;
}

void RegisterWindowClass(HINSTANCE hInstance,
    LPCWSTR lpClassName,
    LPCWSTR lpMenuName,
    LPCWSTR lpIconName,
    WNDPROC lpfnWndProc)
{
  WNDCLASSEX wcex = {sizeof(wcex)};
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = lpfnWndProc;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, lpIconName);
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wcex.lpszMenuName = lpMenuName;
  wcex.lpszClassName = lpClassName;
  RegisterClassEx(&wcex);
}

void ShowContextMenu(HINSTANCE hInstance, HWND hwnd, LPCWSTR lpMenuName, POINT pt) {
  HMENU hMenu = LoadMenu(hInstance, lpMenuName);
  if (hMenu == NULL) return;
  HMENU hSubMenu = GetSubMenu(hMenu, 0);
  if (hSubMenu) {
    // our window must be foreground before calling TrackPopupMenu or the menu will not disappear when the user clicks away
    SetForegroundWindow(hwnd);
    // respect menu drop alignment
    UINT uFlags = TPM_RIGHTBUTTON;
    if (GetSystemMetrics(SM_MENUDROPALIGNMENT) != 0) {
      uFlags |= TPM_RIGHTALIGN;
    } else {
      uFlags |= TPM_LEFTALIGN;
    }
    TrackPopupMenuEx(hSubMenu, uFlags, pt.x, pt.y, hwnd, NULL);
  }
  DestroyMenu(hMenu);
}

DWORD GetOverlappedWindowStyle() {
  return WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
}

HWND MakeOverlappedWindow(HINSTANCE hInstance,
    LPCWSTR lpClassName,
    LPCWSTR lpTitle,
    int width, int height)
{
  DWORD style = GetOverlappedWindowStyle();
  HWND hwnd = CreateWindowEx(0, lpClassName, lpTitle, 
    style,
    CW_USEDEFAULT, CW_USEDEFAULT, width, height,
    NULL, NULL, hInstance, NULL);
  return hwnd;
}

// @see https://docs.microsoft.com/en-us/windows/win32/controls/create-a-button
HWND MakeButton(HINSTANCE hInstance, HWND hwnd, 
    int x, int y, int width, int height,
    LPWSTR lpWindowName, HMENU hMenu) 
{
  return CreateWindow(L"BUTTON", lpWindowName,
      WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
      x, y, width, height,
      hwnd, hMenu, hInstance, NULL);
}

// @see https://docs.microsoft.com/en-us/windows/win32/api/shellapi/ns-shellapi-notifyicondataa
// @see https://github.com/microsoft/Windows-classic-samples/blob/67a8cddc25880ebc64018e833f0bf51589fd4521/Samples/Win7Samples/winui/shell/appshellintegration/NotificationIcon/NotificationIcon.cpp
void RegisterTrayIcon(HINSTANCE hInstance, HWND hwnd, GUID guid,
    UINT uCallbackMessage, LPCWSTR lpIconName, LPCWSTR lpTip)
{
  NOTIFYICONDATA nid = {sizeof(nid)};
  nid.hWnd = hwnd;
  nid.guidItem = guid;
  nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE | NIF_GUID;
  nid.uCallbackMessage = uCallbackMessage;
  nid.hIcon = LoadIcon(hInstance, lpIconName);
  wcscpy(nid.szTip, lpTip);
  Shell_NotifyIcon(NIM_ADD, &nid);
  nid.uVersion = NOTIFYICON_VERSION_4;
  Shell_NotifyIcon(NIM_SETVERSION, &nid);
}

void UnregisterTrayIcon(GUID guid) {
  NOTIFYICONDATA nid = {sizeof(nid)};
  nid.uFlags = NIF_GUID;
  nid.guidItem = guid;
  Shell_NotifyIcon(NIM_DELETE, &nid);
}

static HWND FindSpanwedWorkerWindow() {
  HWND worker = NULL;
  for (;;) {
    worker = FindWindowEx(NULL, worker, L"WorkerW", NULL);
    if (worker == NULL) break;
    if (FindWindowEx(worker, NULL, L"SHELLDLL_DefView", NULL)) {
      return FindWindowEx(NULL, worker, L"WorkerW", NULL);
    }
  }
  return NULL;
}

void ConfigureWallpaperWindow(HWND hwnd) {
  HWND explorer = FindWindow(L"Progman", L"Program Manager");
  // a magic
  // @see https://www.codeproject.com/articles/856020/draw-behind-desktop-icons-in-windows-plus
  ULONG_PTR result = NULL;
  SendMessageTimeout(explorer, 0x052C, NULL, NULL, SMTO_NORMAL, 1000, &result);
  HWND worker = FindSpanwedWorkerWindow();
  SetParent(hwnd, worker);
  // refresh
  InvalidateRect(explorer, NULL, TRUE);
  UpdateWindow(explorer);
  InvalidateRect(worker, NULL, TRUE);
  UpdateWindow(worker);
}

HWND FindWallpaperWindow(LPWSTR lpWindowCaption) {
  HWND worker = FindSpanwedWorkerWindow();
  return FindWindowEx(worker, NULL, NULL, lpWindowCaption);
}

}  // namespace imascs
