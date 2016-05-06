/*
 Copyright (c) 2015, 2016
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

#include "imagine/envi/impl/window_native.h"
#include "imagine/envi/impl/keyboard_native.h"
#include "imagine/envi/impl/mouse_native.h"
#include "imagine/envi/impl/cursor_native.h"

#if defined(IG_WIN64)
# define GCL_HCURSOR GCLP_HCURSOR
# define GWL_USERDATA GWLP_USERDATA
#endif

namespace ig   {
namespace impl {

static std::string ig_window_class = "ig_winclass";

window_native::window_native(const window& ref)
  : ref_{ref}, style_{window::style_t::none}, visibility_{window::visibility_t::hidden},
    mouse_tracked_{false}, handle_{nullptr}, wstyle_{0} {

  if (!reg()) {
    throw std::runtime_error("Failed to register wndclass instance");
  }

  handle_ = CreateWindowEx(0, ig_window_class.data(), "", 0,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                           nullptr, nullptr, GetModuleHandle(nullptr), this);
}

window_native::window_native(const window& ref, const std::string& caption, uint32_t width, uint32_t height, window::style_ft style)
  : ref_{ref}, caption_{caption}, style_{style}, visibility_{window::visibility_t::windowed},
    mouse_tracked_{false}, handle_{nullptr}, wstyle_{0} {

  if (!reg()) {
    throw std::runtime_error("Failed to register wndclass instance");
  }

  wstyle_ = WS_VISIBLE;
  if (style_ & window::style_t::titlebar) {
    wstyle_ |= WS_CAPTION | WS_MINIMIZEBOX;

    if (style_ & window::style_t::closable) {
      wstyle_ |= WS_SYSMENU;
    } if (style_ & window::style_t::resizable) {
      wstyle_ |= WS_MAXIMIZEBOX | WS_SIZEBOX;
    }
  } else {
    wstyle_ |= WS_POPUP;
  }

  auto rect = RECT{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
  AdjustWindowRect(&rect, wstyle_, false);
  width  = rect.right - rect.left; height = rect.bottom - rect.top;

  auto x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
  auto y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
  
  handle_ = CreateWindowEx(0, ig_window_class.data(), caption.data(), wstyle_,
                           x, y, width, height, nullptr, nullptr, GetModuleHandle(nullptr), this);

  auto client = RECT{}, window = RECT{};
  GetClientRect(handle_, &client);
  GetWindowRect(handle_, &window);

  x_ = window.left; y_ = window.top;
  width_ = client.right - client.left; height_ = client.bottom - client.top;
}

LRESULT window_native::internal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  auto keyboard_ev = [&wparam](arg_keyboard::type_t type) -> arg_keyboard {
    return {type, keyboard::modifiers(), static_cast<uint32_t>(wparam), keyboard::key(wparam)};
  };

  auto mouse_move_ev = [&lparam, this](arg_mouse::type_t type, bool enter) -> arg_mouse {
    auto nx = mouse::x(lparam), ny = mouse::y(lparam);
    auto ev = arg_mouse{type, keyboard::modifiers(), mouse::buttons(), nx, ny};
    ev.move.dx = enter ? 0 : nx - ref_.cursor_.native_->x_;
    ev.move.dy = enter ? 0 : ny - ref_.cursor_.native_->y_;
    ref_.cursor_.native_->x_ = nx; ref_.cursor_.native_->y_ = ny;
    return ev;
  };
  auto mouse_ev = [&lparam](arg_mouse::type_t type) -> arg_mouse {
    return {type, keyboard::modifiers(), mouse::buttons(), mouse::x(lparam), mouse::y(lparam)};
  };
  auto mouse_click_ev = [&lparam, &mouse_ev](arg_mouse::type_t type, button_t button) -> arg_mouse {
    auto ev = mouse_ev(type); ev.click.button = button;
    return ev;
  };
  auto mouse_wheel_ev = [&lparam, &wparam, &mouse_ev](arg_mouse::type_t type) -> arg_mouse {
    auto ev = mouse_ev(type); ev.wheel.delta = mouse::wheel_delta(wparam);
    return ev;
  };

  switch (msg) {
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN: 
    ref_.process(keyboard_ev(arg_keyboard::type_t::key_press));
    break;
  case WM_KEYUP:
  case WM_SYSKEYUP: 
    ref_.process(keyboard_ev(arg_keyboard::type_t::key_release));
    break;
  case WM_LBUTTONDOWN: 
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_down, button_t::left));
    break;
  case WM_MBUTTONDOWN: 
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_down, button_t::middle));
    break;
  case WM_RBUTTONDOWN:
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_down, button_t::right));
    break;
  case WM_LBUTTONUP: 
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_up, button_t::left));
    break;
  case WM_MBUTTONUP: 
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_up, button_t::middle));
    break;
  case WM_RBUTTONUP: 
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_up, button_t::right));
    break;
  case WM_LBUTTONDBLCLK: 
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_down, button_t::left));
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_dbl_click, button_t::left));
    break;
  case WM_MBUTTONDBLCLK:
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_down, button_t::middle));
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_dbl_click, button_t::middle));
    break;
  case WM_RBUTTONDBLCLK:
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_down, button_t::right));
    ref_.process(mouse_click_ev(arg_mouse::type_t::mouse_dbl_click, button_t::right));
    break;
  case WM_MOUSEWHEEL: 
    ref_.process(mouse_wheel_ev(arg_mouse::type_t::mouse_wheel));
    break;
  case WM_MOUSELEAVE:
    mouse_tracked_ = false;
    ref_.process(mouse_ev(arg_mouse::type_t::mouse_leave));
    break;
    case WM_MOUSEMOVE:
    if (!mouse_tracked_) {
      mouse::track(handle_); 
      mouse_tracked_ = true;

      ref_.process(mouse_ev(arg_mouse::type_t::mouse_enter));
      ref_.process(mouse_move_ev(arg_mouse::type_t::mouse_move, true));
    } else {
      ref_.process(mouse_move_ev(arg_mouse::type_t::mouse_move, false));
    }
    break;
  case WM_EXITSIZEMOVE: 
    {
      auto client = RECT{}, window = RECT{};
      GetClientRect(handle_, &client);
      GetWindowRect(handle_, &window);

      if (x_ != window.left || y_ != window.top) {
        x_ = window.left; y_ = window.top;
        ref_.process(arg_status{arg_status::type_t::move});
      }

      auto cwidth = client.right - client.left;
      auto cheight = client.bottom - client.top;
      if (width_ != cwidth || height_ != cheight) {
        width_ = cwidth; height_ = cheight;
        ref_.process(arg_status{arg_status::type_t::resize});
      }
    }
    break;
  case WM_CLOSE:
    ref_.process(arg_status{arg_status::type_t::close});
    break;
  case WM_SETCURSOR:
    if (LOWORD(lparam) == HTCLIENT) {
      ref_.cursor_.refresh();
    }
    break;
  default:
    break;
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK window_native::proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  window_native* callback = nullptr;
  if (msg == WM_NCCREATE) {
    auto cs   = reinterpret_cast<CREATESTRUCT*>(lparam);
    callback  = reinterpret_cast<window_native*>(cs->lpCreateParams);
    SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(callback));
  } else {
    callback = reinterpret_cast<window_native*>(GetWindowLongPtr(hwnd, GWL_USERDATA));
  }

  if (callback) {
    return callback->internal(hwnd, msg, wparam, lparam);
  } return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool window_native::reg() {
  auto inst = GetModuleHandle(nullptr);
  auto wc   = WNDCLASSEX{};
  if (GetClassInfoEx(inst, ig_window_class.data(), &wc))
    return true;

  wc.cbSize        = sizeof(WNDCLASSEX);
  wc.style         = CS_DBLCLKS;
  wc.hInstance     = inst;
  wc.lpfnWndProc   = proc;
  wc.lpszClassName = ig_window_class.data();
  wc.lpszMenuName  = nullptr;
  wc.hbrBackground = GetSysColorBrush(COLOR_WINDOW);
  wc.hCursor       = nullptr;
  wc.hIcon         = nullptr;
  wc.hIconSm       = nullptr;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  return RegisterClassEx(&wc) != 0;
}

} // namespace impl

void window::close() {
  DestroyWindow(native_->handle_);
  native_->handle_ = nullptr;
}

void window::show() {
  ShowWindow(native_->handle_, SW_SHOW);
}

void window::hide() {
  ShowWindow(native_->handle_, SW_HIDE);
}

void window::focus() {
  SetForegroundWindow(native_->handle_);
}

void window::move(int32_t x, int32_t y) {
  SetWindowPos(native_->handle_, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void window::resize(uint32_t width, uint32_t height) {
  auto rect = RECT{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
  AdjustWindowRect(&rect, static_cast<DWORD>(GetWindowLongPtr(native_->handle_, GWL_STYLE)), false);

  SetWindowPos(native_->handle_, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
}

bool window::opened() const {
  return native_->handle_ != nullptr;
}

bool window::minimized() const {
  return IsIconic(native_->handle_) == TRUE;
}

bool window::maximized() const {
  return IsZoomed(native_->handle_) == TRUE;
}

bool window::visible() const {
  return IsWindowVisible(native_->handle_) == TRUE;
}

void window::set_fullscreen(bool fullscreen) {
  if (fullscreen) {
    auto minfo = MONITORINFO{};
    minfo.cbSize = sizeof(minfo);
    GetMonitorInfo(MonitorFromWindow(native_->handle_, MONITOR_DEFAULTTONEAREST), &minfo);

    SetWindowLong(native_->handle_, GWL_STYLE, native_->wstyle_ & ~(WS_CAPTION | WS_THICKFRAME));
    SetWindowPos(native_->handle_, nullptr, minfo.rcMonitor.left, minfo.rcMonitor.top,
                 minfo.rcMonitor.right - minfo.rcMonitor.left, minfo.rcMonitor.bottom - minfo.rcMonitor.top,
                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

    native_->visibility_ = visibility_t::fullscreen;
  } else {
    SetWindowLong(native_->handle_, GWL_STYLE, native_->wstyle_);
    SetWindowPos(native_->handle_, nullptr, native_->x_, native_->y_, native_->width_, native_->height_,
                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

    native_->visibility_ = visibility_t::windowed;
  }
}

void window::set_caption(const std::string& caption) {
  SetWindowText(native_->handle_, caption.data());
}

void window::set_parent(const window* parent) {
  auto handle = (parent) ? parent->native_->handle_ : nullptr;
  native_->wstyle_ &= (handle) ? ~WS_POPUP | WS_CHILD : ~WS_CHILD;

  SetWindowLong(native_->handle_, GWL_STYLE, native_->wstyle_);
  SetParent(native_->handle_, handle);
}

} // namespace ig
