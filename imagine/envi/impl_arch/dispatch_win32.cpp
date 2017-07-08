/*
 Copyright (c) 2017
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#include "imagine/envi/impl_arch/dispatch_impl.h"
#include "imagine/envi/impl_arch/window_impl.h"
#include "imagine/envi/dispatch.h"

#if defined(IG_WIN64)
# define GCL_HCURSOR GCLP_HCURSOR
# define GWL_USERDATA GWLP_USERDATA
#endif

namespace ig   {
namespace impl {

dispatch_native::dispatch_native()
  : return_code_{-1}
  , running_{false} {}

HINSTANCE dispatch_native::reg() {
  static HINSTANCE window_inst = GetModuleHandle(nullptr);
  static WNDCLASSEX window_class {};
  if (GetClassInfoEx(window_inst, ig_window_class.c_str(), &window_class))
    return window_inst;

  window_class.cbSize    = sizeof(WNDCLASSEX);
  window_class.hInstance = window_inst;
    window_class.lpfnWndProc   = proc;
    window_class.lpszClassName = ig_window_class.c_str();

  return RegisterClassEx(&window_class) != 0
    ? window_inst
    : nullptr;
}

LRESULT CALLBACK dispatch_native::proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  window_native* callback = nullptr;
  if (msg == WM_NCCREATE) {
    auto cs = reinterpret_cast<CREATESTRUCT*>(lparam);
    callback = reinterpret_cast<window_native*>(cs->lpCreateParams);
    SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(callback));
  } else {
    callback = reinterpret_cast<window_native*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
  }

  if (callback) {
    return callback->internal(hwnd, msg, wparam, lparam);
  } else {
    return DefWindowProc(hwnd, msg, wparam, lparam);
  }
}

} // namespace impl

void dispatch::process_events() {
  for (auto& window : native_->windows_)
    ;

  MSG msg{};
  while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  } tick_();
}

} // namespace ig
