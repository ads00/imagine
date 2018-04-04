/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/envi/impl_arch/dispatch_impl.h"
#include "imagine/envi/impl_arch/widget_impl.h"
#include "imagine/envi/dispatch.h"

#if defined(IG_WIN64)
# define GCL_HCURSOR GCLP_HCURSOR
# define GWL_USERDATA GWLP_USERDATA
#endif

namespace ig   {
namespace impl {

dispatch_impl::dispatch_impl()
  : return_code_{-1}
  , running_{false} {}

HINSTANCE dispatch_impl::reg() {
  static HINSTANCE widget_inst = GetModuleHandle(nullptr);
  static WNDCLASSEX widget_class {};
  if (GetClassInfoEx(widget_inst, ig_widget_class.c_str(), &widget_class))
    return widget_inst;

  widget_class.cbSize    = sizeof(WNDCLASSEX);
  widget_class.hInstance = widget_inst;
    widget_class.lpfnWndProc   = proc;
    widget_class.lpszClassName = ig_widget_class.c_str();

  return RegisterClassEx(&widget_class) != 0
    ? widget_inst
    : nullptr;
}

LRESULT CALLBACK dispatch_impl::proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  widget_impl* callback = nullptr;
  if (msg == WM_NCCREATE) {
    auto cs = reinterpret_cast<CREATESTRUCT*>(lparam);
    callback = reinterpret_cast<widget_impl*>(cs->lpCreateParams);
    SetWindowLongPtr(
      hwnd,
      GWL_USERDATA,
      reinterpret_cast<LONG_PTR>(callback));
  } else {
    callback = reinterpret_cast<widget_impl*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
  }

  if (callback) {
    return callback->internal(
      hwnd,
      msg,
      wparam,
      lparam);
  } else {
    return DefWindowProc(
      hwnd,
      msg,
      wparam,
      lparam);
  }
}

} // namespace impl

void dispatch::process_events() {
  for (auto& widget : native_->widgets_)
    ;

  MSG msg{};
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  } if (native_->running_) tick_();
}

} // namespace ig
