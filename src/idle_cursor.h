
#pragma once

#ifndef UNICODE
#define UNICODE
#endif
#define OEMRESOURCE
#include <windows.h>

#include <optional>
#include <functional>
#include <memory>
#include <vector>

namespace imascs {

VOID CALLBACK IdleInputDetectTimerProc(HWND, UINT, UINT_PTR, DWORD);
LRESULT CALLBACK IdleCursorLowLevelMouseProc(int, WPARAM, LPARAM);

class IdleInputDetect {
 private:
  HWND hwnd_;
  std::optional<UINT_PTR> timer_;
  std::optional<DWORD> previous_last_input_time_;
  std::optional<std::function<void(void)>> callback_;

 public:
  explicit IdleInputDetect(HWND hwnd);
  IdleInputDetect(const IdleInputDetect&) = delete;

  ~IdleInputDetect();

  void OnTimer();
  void OnIdle();

  void RegisterCallback(const std::function<void(void)>& callback) {
    callback_= callback;
  }
};

struct IdleCursorGlobalHookSink {
  virtual ~IdleCursorGlobalHookSink() {}
  virtual void OnMouseMove() = 0;
};

class IdleCursorGlobalHook {
 private:
  std::vector<std::shared_ptr<IdleCursorGlobalHookSink>> sinks_;

 public:
  IdleCursorGlobalHook() {}
  IdleCursorGlobalHook(const IdleCursorGlobalHook&) = delete;
  
  static IdleCursorGlobalHook& Instance() {
    static IdleCursorGlobalHook instance;
    return instance;
  }

  void AddSinkOneshot(const std::shared_ptr<IdleCursorGlobalHookSink>& sink);
  void OnMouseMove();
};

class IdleCursorBecomeActiveDetect
  : public IdleCursorGlobalHookSink,
    public std::enable_shared_from_this<IdleCursorBecomeActiveDetect> 
{
 private:
  std::optional<HHOOK> hook_;
  std::optional<std::function<void(void)>> callback_;

 public:
  static std::shared_ptr<IdleCursorBecomeActiveDetect> Create() {
    auto instance = std::make_shared<IdleCursorBecomeActiveDetect>();
    instance->Initialize();
    return instance;
  }

  IdleCursorBecomeActiveDetect();
  IdleCursorBecomeActiveDetect(const IdleCursorBecomeActiveDetect&) = delete;
  virtual ~IdleCursorBecomeActiveDetect();

  void Initialize();

  void RegisterCallback(const std::function<void(void)>& callback) {
    callback_= callback;
  }

  virtual void OnMouseMove();
};

class BlankCursorBuilder {
 private:
  HINSTANCE hInstance_;
  int cx_;
  int cy_;
  UINT8* plane_and_;
  UINT8* plane_xor_;

 public:
  BlankCursorBuilder(HINSTANCE hInstance);
  BlankCursorBuilder(const BlankCursorBuilder&) = delete;

  ~BlankCursorBuilder();

  HCURSOR CreateBlankCursor();
};

class IdleCursorWatcher 
  : public std::enable_shared_from_this<IdleCursorWatcher>
{
 private:
  HWND hwnd_;
  std::optional<std::unique_ptr<IdleInputDetect>> idle_input_detect_;
  std::optional<std::shared_ptr<IdleCursorBecomeActiveDetect>> become_active_detect_;
  std::unique_ptr<BlankCursorBuilder> blank_cursor_builder_;
  bool is_cursor_hidden_;

  void SwitchToWatchIdle();
  void SwitchToWatchActive();

  void OnIdle();
  void OnActive();

 public:
  static std::shared_ptr<IdleCursorWatcher> Create(HINSTANCE hInstance, HWND hwnd) {
    auto instance = std::make_shared<IdleCursorWatcher>(hInstance, hwnd);
    instance->Initialize();
    return instance;
  }

  explicit IdleCursorWatcher(HINSTANCE hInstance, HWND hwnd);

  IdleCursorWatcher(const IdleCursorWatcher&) = delete;
  ~IdleCursorWatcher();

  void Initialize();
};

}  // namespace imascs
