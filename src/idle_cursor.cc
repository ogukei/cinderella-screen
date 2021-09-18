
#include "idle_cursor.h"

#include <iostream>
#include <algorithm>
#include <cstring>

#include "defines.hpp"
#include "actions.h"
#include "window_helper.h"

namespace imascs {

VOID CALLBACK IdleInputDetectTimerProc(HWND hwnd, UINT, UINT_PTR id, DWORD) {
  // check hwnd is still alive
  if (!IsWindow(hwnd)) return;
  IdleInputDetect* instance = reinterpret_cast<IdleInputDetect*>(id);
  if (instance == nullptr) return;
  instance->OnTimer();
}

LRESULT CALLBACK IdleCursorLowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam) {
  LPMSLLHOOKSTRUCT hook = (LPMSLLHOOKSTRUCT)lParam;
  if (wParam == WM_MOUSEMOVE && nCode == HC_ACTION) {
    IdleCursorGlobalHook::Instance().OnMouseMove();
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

static std::optional<DWORD> FetchLastInputTime() {
  // note that time is represented as 32 bit integer
  LASTINPUTINFO info = {sizeof(info)};
  if (!GetLastInputInfo(&info)) return std::nullopt;
  return info.dwTime;
}

// IdleInputDetect
IdleInputDetect::IdleInputDetect(HWND hwnd)
  : hwnd_(NULL),
    timer_(0)
{
  if (hwnd == NULL) return;
  UINT_PTR id = reinterpret_cast<UINT_PTR>(this);
  hwnd_ = hwnd;
  timer_ = id;
  previous_last_input_time_ = FetchLastInputTime();
  SetTimer(hwnd, id, 2000, IdleInputDetectTimerProc);
}

IdleInputDetect::~IdleInputDetect() {
  if (timer_.has_value()) {
    KillTimer(hwnd_, timer_.value());
  }
}

void IdleInputDetect::OnTimer() {
  auto current = FetchLastInputTime();
  if (current.has_value() && previous_last_input_time_.has_value()) {
    // check every time window
    if (current.value() == previous_last_input_time_.value()) {
      OnIdle();
    }
  }
  previous_last_input_time_ = current;
}

void IdleInputDetect::OnIdle() {
  if (timer_.has_value()) {
    KillTimer(hwnd_, timer_.value());
    timer_ = std::nullopt;
  }
  if (callback_.has_value()) {
    auto& callback = callback_.value();
    callback();
    callback_ = std::nullopt;
  }
}

// IdleCursorGlobalHook
void IdleCursorGlobalHook::AddSinkOneshot(const std::shared_ptr<IdleCursorGlobalHookSink>& sink) {
  bool contains = std::any_of(sinks_.begin(), sinks_.end(),
    [&sink](const std::shared_ptr<IdleCursorGlobalHookSink>& v) {
      return v.get() == sink.get();
    });
  if (contains) return;
  sinks_.push_back(sink);
}

void IdleCursorGlobalHook::OnMouseMove() {
  auto sinks = sinks_;
  sinks_.clear();
  for (auto& v : sinks) {
    v->OnMouseMove();
  }
}

// IdleCursorBecomeActiveDetect
IdleCursorBecomeActiveDetect::IdleCursorBecomeActiveDetect() {}

void IdleCursorBecomeActiveDetect::Initialize() {
  auto shared = std::static_pointer_cast<IdleCursorGlobalHookSink>(shared_from_this());
  IdleCursorGlobalHook::Instance().AddSinkOneshot(shared);
  HHOOK hook = SetWindowsHookExA(WH_MOUSE_LL, IdleCursorLowLevelMouseProc, NULL, 0);
  if (hook != NULL) {
    hook_ = hook;
  }
}

IdleCursorBecomeActiveDetect::~IdleCursorBecomeActiveDetect() {
  if (hook_.has_value()) {
    UnhookWindowsHookEx(hook_.value());
  }
}

void IdleCursorBecomeActiveDetect::OnMouseMove() {
  if (hook_.has_value()) {
    UnhookWindowsHookEx(hook_.value());
    hook_ = std::nullopt;
  }
  if (callback_.has_value()) {
    auto& callback = callback_.value();
    callback();
    callback_ = std::nullopt;
  }
}

// BlankCursorBuilder
BlankCursorBuilder::BlankCursorBuilder(HINSTANCE hInstance) 
  : hInstance_(hInstance),
    cx_(0), 
    cy_(0), 
    plane_and_(nullptr), 
    plane_xor_(nullptr)
{
  int cx = GetSystemMetrics(SM_CXCURSOR);
  int cy = GetSystemMetrics(SM_CYCURSOR);
  if (cx == 0 || cy == 0) return;
  size_t size = cy * cx;
  UINT8* plane_and = new UINT8[size];
  UINT8* plane_xor = new UINT8[size];
  std::memset(plane_and, 0xff, size);
  std::memset(plane_xor, 0x00, size);
  plane_and_ = plane_and;
  plane_xor_ = plane_xor;
  cx_ = cx;
  cy_ = cy;
}

BlankCursorBuilder::~BlankCursorBuilder() {
  delete[] plane_and_;
  delete[] plane_xor_;
}

HCURSOR BlankCursorBuilder::CreateBlankCursor() {
  return CreateCursor(hInstance_, 0, 0, cx_, cy_, plane_and_, plane_xor_);
}

// IdleCursorWatcher
IdleCursorWatcher::IdleCursorWatcher(HINSTANCE hInstance, HWND hwnd)
  : hwnd_(hwnd),
    is_cursor_hidden_(false),
    blank_cursor_builder_(std::make_unique<BlankCursorBuilder>(hInstance))
{
}

void IdleCursorWatcher::Initialize() {
  SwitchToWatchIdle();
}

IdleCursorWatcher::~IdleCursorWatcher() {
  if (is_cursor_hidden_) {
    SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
  }
}

void IdleCursorWatcher::SwitchToWatchIdle() {
  become_active_detect_ = std::nullopt;
  std::weak_ptr<IdleCursorWatcher> weak = shared_from_this();
  auto detect = std::make_unique<IdleInputDetect>(hwnd_);
  detect->RegisterCallback([weak]{
    auto shared = weak.lock();
    if (!shared) return;
    shared->OnIdle();
    shared->SwitchToWatchActive();
  });
  idle_input_detect_ = std::move(detect);
}

void IdleCursorWatcher::SwitchToWatchActive() {
  idle_input_detect_ = std::nullopt;
  // immediately show cursor on become active
  std::weak_ptr<IdleCursorWatcher> weak = shared_from_this();
  auto detect = IdleCursorBecomeActiveDetect::Create();
  detect->RegisterCallback([weak]{
    auto shared = weak.lock();
    if (!shared) return;
    shared->OnActive();
    shared->SwitchToWatchIdle();
  });
  become_active_detect_ = std::move(detect);
}

void IdleCursorWatcher::OnIdle() {
  if (is_cursor_hidden_) return;
  is_cursor_hidden_ = true;
  SetSystemCursor(blank_cursor_builder_->CreateBlankCursor(), OCR_NORMAL);
  SetSystemCursor(blank_cursor_builder_->CreateBlankCursor(), OCR_APPSTARTING);
  SetSystemCursor(blank_cursor_builder_->CreateBlankCursor(), OCR_WAIT);
}

void IdleCursorWatcher::OnActive() {
  if (!is_cursor_hidden_) return;
  is_cursor_hidden_ = false;
  SystemParametersInfo(SPI_SETCURSORS, 0, NULL, 0);
}

}  // namespace imascs
