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

#include "imagine/interact/impl/window_native.h"
#include "imagine/interact/impl/keyboard_native.h"
#include "imagine/interact/impl/mouse_native.h"
#include "imagine/interact/impl/cursor_native.h"

#if defined(IG_WIN64)
# define GCL_HCURSOR GCLP_HCURSOR
# define GWL_USERDATA GWLP_USERDATA
#endif

namespace ig   {
namespace impl {

static std::string ig_window_class = "ig_winclass";

window_native::window_native(const window* ref)
  : ref_{ref}, style_{window::style_t::none}, visibility_{window::visibility_t::hidden},
    mouse_tracked_{false}, cursor_{cursor::shape_t::none}, events_{std::make_shared<events>()}, 
    handle_ {nullptr}, wstyle_{0}
{
  if (!reg())
  {
    throw std::runtime_error
      ("Failed to register wndclass instance");
  }

  handle_ = CreateWindowEx(0, ig_window_class.data(), "", 0,
                           CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                           nullptr, nullptr, GetModuleHandle(nullptr), this);
}

window_native::window_native(const window* ref, const std::string& caption, unsigned int width, unsigned int height, window::style_ft style)
  : ref_{ref}, caption_{caption}, style_{style}, visibility_{window::visibility_t::windowed},
    mouse_tracked_{false}, cursor_{cursor::shape_t::arrow}, events_{std::make_shared<events>()}, 
    handle_{nullptr}, wstyle_{0}
{
  if (!reg())
  {
    throw std::runtime_error
      ("Failed to register wndclass instance");
  }

  wstyle_ = WS_VISIBLE;
  if (style_ & window::style_t::titlebar)
  {
    wstyle_ |= WS_CAPTION | WS_MINIMIZEBOX;
    if (style_ & window::style_t::closable)
      wstyle_ |= WS_SYSMENU;

    if (style_ & window::style_t::resizable)
      wstyle_ |= WS_MAXIMIZEBOX | WS_SIZEBOX;
  }
  else
    wstyle_ |= WS_POPUP;

  RECT rect{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
  AdjustWindowRect(&rect, wstyle_, false);
  width  = rect.right - rect.left;
  height = rect.bottom - rect.top;

  const int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
  const int y = (GetSystemMetrics(SM_CYSCREEN) - width) / 2;
  
  handle_ = CreateWindowEx(0, ig_window_class.data(), caption.data(), wstyle_,
                           x, y, width, height,
                           nullptr, nullptr, GetModuleHandle(nullptr), this);

  RECT client, window;
  GetClientRect(handle_, &client);
  GetWindowRect(handle_, &window);

  x_ = window.left;
  y_ = window.top;
  width_ = client.right - client.left;
  height_ = client.bottom - client.top;
}

LRESULT window_native::internal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  const auto keyboard_ev = [&wparam](arg_keyboard::type_t type)
  {
    arg_keyboard ev;
    ev.type = type;

    ev.modifiers = keyboard::modifiers();
    ev.vk = static_cast<uint32_t>(wparam);
    ev.key = keyboard::key(wparam);

    return ev;
  };

  const auto mouse_ev = [&lparam](arg_mouse::type_t type)
  {
    arg_mouse ev;
    ev.type = type;

    ev.modifiers = keyboard::modifiers();
    ev.buttons = mouse::buttons();

    ev.x = mouse::x(lparam);
    ev.y = mouse::y(lparam);

    return ev;
  };

  switch (msg)
  {
  case WM_KEYDOWN:
  case WM_SYSKEYDOWN:
  {
    const auto ev = keyboard_ev(arg_keyboard::type_t::key_press);
    events_->process(ev);
    break;
  }

  case WM_KEYUP:
  case WM_SYSKEYUP:
  {
    const auto ev = keyboard_ev(arg_keyboard::type_t::key_release);
    events_->process(ev);
    break;
  }

  case WM_LBUTTONDOWN:
  {
    auto ev = mouse_ev(arg_mouse::type_t::mouse_down);
    ev.click.button = button_t::left;

    events_->process(ev);
    break;
  }

  case WM_MBUTTONDOWN:
  {
    auto ev = mouse_ev(arg_mouse::type_t::mouse_down);
    ev.click.button = button_t::middle;

    events_->process(ev);
    break;
  }

  case WM_RBUTTONDOWN:
  {
    auto ev = mouse_ev(arg_mouse::type_t::mouse_down);
    ev.click.button = button_t::right;

    events_->process(ev);
    break;
  }

  case WM_LBUTTONUP:
  {
    auto ev = mouse_ev(arg_mouse::type_t::mouse_up);
    ev.click.button = button_t::left;

    events_->process(ev);
    break;
  }

  case WM_MBUTTONUP:
  {
    auto ev = mouse_ev(arg_mouse::type_t::mouse_up);
    ev.click.button = button_t::middle;

    events_->process(ev);
    break;
  }

  case WM_RBUTTONUP:
  {
    auto ev = mouse_ev(arg_mouse::type_t::mouse_up);
    ev.click.button = button_t::right;

    events_->process(ev);
    break;
  }

  case WM_LBUTTONDBLCLK:
  {
    auto ev = mouse_ev(arg_mouse::type_t::mouse_down);
    ev.click.button = button_t::left;

    events_->process(ev);

    ev.type = arg_mouse::type_t::mouse_dbl_click;
    events_->process(ev);
    break;
  }

  case WM_MBUTTONDBLCLK:
  {
    auto ev = mouse_ev(arg_mouse::type_t::mouse_down);
    ev.click.button = button_t::middle;

    events_->process(ev);

    ev.type = arg_mouse::type_t::mouse_dbl_click;
    events_->process(ev);
    break;
  }

  case WM_RBUTTONDBLCLK:
  {
    auto ev = mouse_ev(arg_mouse::type_t::mouse_down);
    ev.click.button = button_t::right;

    events_->process(ev);

    ev.type = arg_mouse::type_t::mouse_dbl_click;
    events_->process(ev);
    break;
  }

  case WM_MOUSEMOVE:
  {
    if (!mouse_tracked_)
    {
      mouse::track(handle_);
      mouse_tracked_ = true;

      auto ev = mouse_ev(arg_mouse::type_t::mouse_enter);
      events_->process(ev);
    }

    auto ev = mouse_ev(arg_mouse::type_t::mouse_move);
    events_->process(ev);
    break;
  }

  case WM_MOUSELEAVE:
  {
    mouse_tracked_ = false;

    auto ev = mouse_ev(arg_mouse::type_t::mouse_leave);
    events_->process(ev);
    break;
  }

  case WM_MOUSEWHEEL:
  {
    auto ev = mouse_ev(arg_mouse::type_t::mouse_wheel);
    ev.wheel.delta = mouse::wheel_delta(wparam);

    events_->process(ev);
    break;
  }

  case WM_EXITSIZEMOVE:
  {
    RECT client, window;
    GetClientRect(handle_, &client);
    GetWindowRect(handle_, &window);

    if (x_ != window.left || y_ != window.top)
    {
      x_ = window.left; 
      y_ = window.top;

      arg_status ev;
      ev.type = arg_status::type_t::move;

      events_->process(ev);
    }

    const unsigned int cwidth  = client.right - client.left;
    const unsigned int cheight = client.bottom - client.top;
    if (width_ != cwidth || height_ != cheight)
    {
      width_ = cwidth; 
      height_ = cheight;

      arg_status ev;
      ev.type = arg_status::type_t::resize;

      events_->process(ev);
    }
    break;
  }

  case WM_CLOSE:
  {
    arg_status ev;
    ev.type = arg_status::type_t::close;

    events_->process(ev);
    break;
  }

  case WM_SETCURSOR:
  {
    if (LOWORD(lparam) == HTCLIENT)
      cursor_.refresh();

    break;
  }

  default:
    break;
  }

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK window_native::proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
  window_native* callback;
  if (msg == WM_NCCREATE)
  {
    CREATESTRUCT* cs{reinterpret_cast<CREATESTRUCT*>(lparam)};
    callback = reinterpret_cast<window_native*>(cs->lpCreateParams);
    SetWindowLongPtr(hwnd, GWL_USERDATA, reinterpret_cast<LONG_PTR>(callback));
  }
  else
    callback = reinterpret_cast<window_native*>(GetWindowLongPtr(hwnd, GWL_USERDATA));

  if (callback)
    return callback->internal(hwnd, msg, wparam, lparam);

  return DefWindowProc(hwnd, msg, wparam, lparam);
}

bool window_native::reg()
{
  const HINSTANCE inst{GetModuleHandle(nullptr)};

  WNDCLASSEX wc;
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

  const ATOM atom{RegisterClassEx(&wc)};
  return atom != 0;
}

} // namespace impl

void window::close()
{
  DestroyWindow(native_->handle_);
  native_->handle_ = nullptr;
}

void window::show()
{
  ShowWindow(native_->handle_, SW_SHOW);
}

void window::hide()
{
  ShowWindow(native_->handle_, SW_HIDE);
}

void window::focus()
{
  SetForegroundWindow(native_->handle_);
}

void window::move(int x, int y)
{
  SetWindowPos(native_->handle_, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void window::resize(unsigned int width, unsigned int height)
{
  RECT rect{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
  AdjustWindowRect(&rect, static_cast<DWORD>(GetWindowLongPtr(native_->handle_, GWL_STYLE)), false);

  SetWindowPos(native_->handle_, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE | SWP_NOZORDER);
}

bool window::opened() const
{
  return native_->handle_ != nullptr;
}

bool window::minimized() const
{
  return IsIconic(native_->handle_) == TRUE;
}

bool window::maximized() const
{
  return IsZoomed(native_->handle_) == TRUE;
}

bool window::visible() const
{
  return IsWindowVisible(native_->handle_) == TRUE;
}

void window::set_parent(const window* parent)
{
  const auto handle = (parent) ? parent->native_->handle_ : nullptr;
  native_->wstyle_ &= (handle) ? ~WS_POPUP | WS_CHILD : ~WS_CHILD;

  SetWindowLong(native_->handle_, GWL_STYLE, native_->wstyle_);
  SetParent(native_->handle_, handle);
}

void window::set_fullscreen(bool fullscreen)
{
  if (fullscreen)
  {
    SetWindowLong(native_->handle_, GWL_STYLE, native_->wstyle_ & ~(WS_CAPTION | WS_THICKFRAME));

    MONITORINFO minfo;
    minfo.cbSize = sizeof(minfo);
    GetMonitorInfo(MonitorFromWindow(native_->handle_, MONITOR_DEFAULTTONEAREST), &minfo);

    SetWindowPos(native_->handle_, nullptr, minfo.rcMonitor.left, minfo.rcMonitor.top,
                 minfo.rcMonitor.right - minfo.rcMonitor.left, minfo.rcMonitor.bottom - minfo.rcMonitor.top,
                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

    native_->visibility_ = visibility_t::fullscreen;
  }
  else
  {
    SetWindowLong(native_->handle_, GWL_STYLE, native_->wstyle_);
    SetWindowPos(native_->handle_, nullptr, native_->x_, native_->y_, native_->width_, native_->height_,
                 SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

    native_->visibility_ = visibility_t::windowed;
  }
}

void window::set_caption(const std::string& caption)
{
  SetWindowText(native_->handle_, caption.data());
}

} // namespace ig
