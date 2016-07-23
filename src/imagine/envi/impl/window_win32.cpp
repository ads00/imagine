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

#include "imagine/envi/impl/window_impl.h"
#include "imagine/envi/impl/keyboard_impl.h"
#include "imagine/envi/impl/mouse_impl.h"
#include "imagine/envi/impl/cursor_impl.h"

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
    throw std::runtime_error{"Failed to register wndclass instance"};
  }

  handle_ = CreateWindowEx(0, ig_window_class.data(), "", 0,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                           nullptr, nullptr, GetModuleHandle(nullptr), this);
}

window_native::window_native(const window& ref, const std::string& caption, uint32_t width, uint32_t height, window::style_flags style)
  : ref_{ref}, caption_{caption}, style_{style}, visibility_{window::visibility_t::windowed},
    mouse_tracked_{false}, handle_{nullptr}, wstyle_{0} {
  if (!reg()) {
    throw std::runtime_error{"Failed to register wndclass instance"};
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

  RECT adjrect{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
  AdjustWindowRect(&adjrect, wstyle_, false);
  auto cwidth = adjrect.right - adjrect.left, cheight = adjrect.bottom - adjrect.top;

  auto cx = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
  auto cy = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
  
  handle_ = CreateWindowEx(0, ig_window_class.data(), caption.data(), wstyle_,
                           cx, cy, cwidth, cheight, nullptr, nullptr, GetModuleHandle(nullptr), this);

  RECT clirect{}, winrect{};
  GetClientRect(handle_, &clirect);
  GetWindowRect(handle_, &winrect);

  width_ = clirect.right - clirect.left, height_ = clirect.bottom - clirect.top;
  x_ = winrect.left, y_ = winrect.top;
}

LRESULT window_native::internal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  auto keyboard_ev = [&wparam](event_keyboard::type_t type) -> event_keyboard {
    return {type, keyboard::impl::modifiers(), keyboard::impl::key(wparam), static_cast<uint32_t>(wparam)};
  };

  auto mouse_ev = [&lparam](event_mouse::type_t type) -> event_mouse {
    return {type, keyboard::impl::modifiers(),
            mouse::impl::buttons(), mouse::impl::x(lparam), mouse::impl::y(lparam)};
  };
  auto mouse_click_ev = [&lparam, &mouse_ev](event_mouse::type_t type, mouse::button_t button) -> event_mouse {
    auto ev = mouse_ev(type); ev.click.button = button;
    return ev;
  };
  auto mouse_wheel_ev = [&lparam, &wparam, &mouse_ev](event_mouse::type_t type) -> event_mouse {
    auto ev = mouse_ev(type); ev.wheel.delta = mouse::impl::wheel_delta(wparam);
    return ev;
  };
  auto mouse_move_ev = [&lparam, this, &mouse_ev](event_mouse::type_t type, bool entered) -> event_mouse {
    auto ev = mouse_ev(type);
    if (ev.x != ref_.cursor_.native_->x_ || ev.y != ref_.cursor_.native_->y_) {
      ev.move.dx = entered ?
        0 :
        ev.x - ref_.cursor_.native_->x_,
        ev.move.dy = entered ?
        0 :
        ev.y - ref_.cursor_.native_->y_;
      ref_.cursor_.native_->x_ = ev.x, ref_.cursor_.native_->y_ = ev.y;
    } else {
      ev.type = event_mouse::none;
    }
    return ev;
  };

  switch (msg) {
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN: 
    ref_.process(keyboard_ev(event_keyboard::pressed));
    break;
  case WM_KEYUP:
  case WM_SYSKEYUP: 
    ref_.process(keyboard_ev(event_keyboard::released));
    break;
  case WM_LBUTTONDOWN: 
    ref_.process(mouse_click_ev(event_mouse::pressed, mouse::button_t::left));
    break;
  case WM_MBUTTONDOWN: 
    ref_.process(mouse_click_ev(event_mouse::pressed, mouse::button_t::middle));
    break;
  case WM_RBUTTONDOWN:
    ref_.process(mouse_click_ev(event_mouse::pressed, mouse::button_t::right));
    break;
  case WM_LBUTTONUP: 
    ref_.process(mouse_click_ev(event_mouse::released, mouse::button_t::left));
    break;
  case WM_MBUTTONUP: 
    ref_.process(mouse_click_ev(event_mouse::released, mouse::button_t::middle));
    break;
  case WM_RBUTTONUP: 
    ref_.process(mouse_click_ev(event_mouse::released, mouse::button_t::right));
    break;
  case WM_LBUTTONDBLCLK: 
    ref_.process(mouse_click_ev(event_mouse::pressed, mouse::button_t::left));
    ref_.process(mouse_click_ev(event_mouse::dbl_clicked, mouse::button_t::left));
    break;
  case WM_MBUTTONDBLCLK:
    ref_.process(mouse_click_ev(event_mouse::pressed, mouse::button_t::middle));
    ref_.process(mouse_click_ev(event_mouse::dbl_clicked, mouse::button_t::middle));
    break;
  case WM_RBUTTONDBLCLK:
    ref_.process(mouse_click_ev(event_mouse::pressed, mouse::button_t::right));
    ref_.process(mouse_click_ev(event_mouse::dbl_clicked, mouse::button_t::right));
    break;
  case WM_MOUSEWHEEL: 
    ref_.process(mouse_wheel_ev(event_mouse::wheeled));
    break;
  case WM_MOUSELEAVE:
    mouse_tracked_ = false;
    ref_.process(mouse_ev(event_mouse::leaved));
    break;
    case WM_MOUSEMOVE:
    if (!mouse_tracked_) {
      mouse::impl::track(handle_); 
      mouse_tracked_ = true;

      ref_.process(mouse_ev(event_mouse::entered));
      ref_.process(mouse_move_ev(event_mouse::moved, true));
    } else {
      ref_.process(mouse_move_ev(event_mouse::moved, false));
    }
    break;
  case WM_EXITSIZEMOVE: 
    {
      RECT clirect{}, winrect{};
      GetClientRect(handle_, &clirect);
      GetWindowRect(handle_, &winrect);

      auto cwidth = clirect.right - clirect.left, cheight = clirect.bottom - clirect.top;
      if (width_ != cwidth || height_ != cheight) {
        width_ = cwidth, height_ = cheight;
        ref_.process(event_status{event_status::resized});
      }

      if (x_ != winrect.left || y_ != winrect.top) {
        x_ = winrect.left, y_ = winrect.top;
        ref_.process(event_status{event_status::moved});
      }
    }
    break;
  case WM_CLOSE:
    ref_.process(event_status{event_status::closed});
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

bool window_native::reg() {
  WNDCLASSEX window_class{};
  if (GetClassInfoEx(GetModuleHandle(nullptr), ig_window_class.c_str(), &window_class))
    return true;

  window_class = {
    sizeof(WNDCLASSEX),             // UINT         cbSize;
    CS_DBLCLKS,                     // UINT         style;
    proc,                           // WNDPROC      lpfnWndProc;
    0,                              // int          cbClsExtra;
    0,                              // int          cbWndExtra;
    GetModuleHandle(nullptr),       // HINSTANCE    hInstance;
    nullptr,                        // HICON        hIcon;
    nullptr,                        // HCURSOR      hCursor;
    GetSysColorBrush(COLOR_WINDOW), // HBRUSH       hbrBackground;
    nullptr,                        // LPCTSTR      lpszMenuName;
    ig_window_class.data(),         // LPCTSTR      lpszClassName;
    nullptr                         // HICON        hIconSm;
  };
  return RegisterClassEx(&window_class) != 0;
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

void window::resize(uint32_t width, uint32_t height) {
  RECT winrect{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
  AdjustWindowRect(&winrect, static_cast<DWORD>(GetWindowLongPtr(native_->handle_, GWL_STYLE)), false);
  SetWindowPos(native_->handle_, nullptr, 0, 0, winrect.right - winrect.left, winrect.bottom - winrect.top, SWP_NOMOVE | SWP_NOZORDER);
}

void window::move(int32_t x, int32_t y) {
  SetWindowPos(native_->handle_, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
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

auto window::handle() const -> window_handle* {
  return reinterpret_cast<window_handle*>(native_->handle_);
}

void window::set_fullscreen(bool fullscreen) {
  if (fullscreen) {
    MONITORINFO minfo{};
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
  auto handle = parent ? 
    parent->native_->handle_ : 
    nullptr;
  native_->wstyle_ &= handle ? 
    ~WS_POPUP | WS_CHILD : 
    ~WS_CHILD;

  SetWindowLong(native_->handle_, GWL_STYLE, native_->wstyle_);
  SetParent(native_->handle_, handle);
}

} // namespace ig
