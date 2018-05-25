/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/envi/impl_arch/widget_impl.h"
#include "imagine/envi/impl_arch/dispatch_impl.h"
#include "imagine/envi/impl_arch/cursor_impl.h"
#include "imagine/envi/impl_arch/interface_keyboard_impl.h"
#include "imagine/envi/impl_arch/interface_mouse_impl.h"

namespace ig   {
namespace impl {

widget_impl::widget_impl(const widget& ref)
  : ref_{ref}
  , styles_{widget_style::none}
  , visibility_{widget_visibility::hidden}
  , mouse_tracked_{false}
  , wstyle_{0}
  , instance_{dispatch_impl::reg()}
  , handle_{nullptr} {

  if (!instance_) {
    throw std::runtime_error{"Failed to register wndclass instance"};
  }

  handle_ =
    CreateWindowEx(
      0, ig_widget_class.data(),
      "",
      0,
      0, 0,
      0, 0,
      nullptr, nullptr,
      instance_, this);
}

widget_impl::widget_impl(const widget& ref, widget_styles styles, const std::string& caption, uint32_t w, uint32_t h)
  : ref_{ref}
  , styles_{styles}
  , visibility_{widget_visibility::windowed}
  , caption_{caption}
  , mouse_tracked_{false}
  , wstyle_{0}
  , instance_{dispatch_impl::reg()}
  , handle_{nullptr} {

  if (!instance_) {
    throw std::runtime_error{"Failed to register wndclass instance"};
  }

  wstyle_ = WS_VISIBLE;
  if (styles_ & widget_style::titlebar) {
    wstyle_ |= WS_CAPTION | WS_MINIMIZEBOX;
    if (styles_ & widget_style::closable)
      wstyle_ |= WS_SYSMENU;
    if (styles_ & widget_style::resizable)
      wstyle_ |= WS_MAXIMIZEBOX | WS_SIZEBOX;
  } else wstyle_ |= WS_POPUP;

  RECT adjrect{0, 0, static_cast<LONG>(w), static_cast<LONG>(h)};
  AdjustWindowRect(
    &adjrect,
    wstyle_,
    false);

  auto cwidth = adjrect.right - adjrect.left, cheight = adjrect.bottom - adjrect.top;
  auto cx = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
  auto cy = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

  handle_ =
    CreateWindowExA(
      0, ig_widget_class.data(),
      caption.data(),
      wstyle_,
      cx, cy,
      cwidth, cheight,
      nullptr, nullptr,
      instance_, this);

  RECT clirect, winrect;
  GetClientRect(
    handle_,
    &clirect);
  GetWindowRect(
    handle_,
    &winrect);

  w_ = clirect.right - clirect.left, h_ = clirect.bottom - clirect.top;
  x_ = winrect.left, y_ = winrect.top;
}

LRESULT widget_impl::internal(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  // keyboard event generator
  auto keyboard_ev = [&wparam](keyboard_event::type_t type) -> keyboard_event {
    return
    {type, keyboard::impl::get_modifiers(), keyboard::impl::get_key(wparam), static_cast<uint32_t>(wparam)}; };

  // mouse event generator
  auto mouse_ev = [&lparam](mouse_event::type_t type) -> mouse_event {
    return
    {type, keyboard::impl::get_modifiers(), mouse::impl::get_buttons(), mouse::impl::get_x(lparam), mouse::impl::get_y(lparam)}; };
  auto mouse_click_ev = [&lparam, &mouse_ev](mouse_event::type_t type, mouse::button button) -> mouse_event {
    auto ev = mouse_ev(type); ev.click.button = button;
    return ev; };
  auto mouse_wheel_ev = [&lparam, &wparam, &mouse_ev](mouse_event::type_t type) -> mouse_event {
    auto ev = mouse_ev(type); ev.wheel.delta = mouse::impl::get_wheel_delta(wparam);
    return ev; };
  auto mouse_move_ev = [this, &lparam, &mouse_ev](mouse_event::type_t type, bool entered) -> mouse_event {
    auto ev = mouse_ev(type);
    if (ev.x != ref_.cursor_.native_->x_ || ev.y != ref_.cursor_.native_->y_) {
      ev.move.dx = entered
        ? 0
        : ev.x - ref_.cursor_.native_->x_,
      ev.move.dy = entered
        ? 0
        : ev.y - ref_.cursor_.native_->y_;
      ref_.cursor_.native_->x_ = ev.x, ref_.cursor_.native_->y_ = ev.y;
    } else {
      ev.type = mouse_event::none;
    } return ev;
  };

  switch (msg) {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
      ref_.keyboard(keyboard_ev(keyboard_event::pressed));
      break;
    case WM_KEYUP:
    case WM_SYSKEYUP:
      ref_.keyboard(keyboard_ev(keyboard_event::released));
      break;
    case WM_LBUTTONDOWN:
      ref_.mouse(mouse_click_ev(mouse_event::pressed, mouse::button::left));
      break;
    case WM_MBUTTONDOWN:
      ref_.mouse(mouse_click_ev(mouse_event::pressed, mouse::button::middle));
      break;
    case WM_RBUTTONDOWN:
      ref_.mouse(mouse_click_ev(mouse_event::pressed, mouse::button::right));
      break;
    case WM_LBUTTONUP:
      ref_.mouse(mouse_click_ev(mouse_event::released, mouse::button::left));
      break;
    case WM_MBUTTONUP:
      ref_.mouse(mouse_click_ev(mouse_event::released, mouse::button::middle));
      break;
    case WM_RBUTTONUP:
      ref_.mouse(mouse_click_ev(mouse_event::released, mouse::button::right));
      break;
    case WM_LBUTTONDBLCLK:
      ref_.mouse(mouse_click_ev(mouse_event::pressed, mouse::button::left));
      ref_.mouse(mouse_click_ev(mouse_event::dbl_clicked, mouse::button::left));
      break;
    case WM_MBUTTONDBLCLK:
      ref_.mouse(mouse_click_ev(mouse_event::pressed, mouse::button::middle));
      ref_.mouse(mouse_click_ev(mouse_event::dbl_clicked, mouse::button::middle));
      break;
    case WM_RBUTTONDBLCLK:
      ref_.mouse(mouse_click_ev(mouse_event::pressed, mouse::button::right));
      ref_.mouse(mouse_click_ev(mouse_event::dbl_clicked, mouse::button::right));
      break;
    case WM_MOUSEWHEEL:
      ref_.mouse(mouse_wheel_ev(mouse_event::wheeled));
      break;
    case WM_MOUSELEAVE:
      mouse_tracked_ = false;
      ref_.mouse(mouse_ev(mouse_event::leaved));
      break;
    case WM_MOUSEMOVE:
    {
      if (!mouse_tracked_) {
        mouse::impl::track(handle_);
        mouse_tracked_ = true;

        ref_.mouse(mouse_ev(mouse_event::entered));
        ref_.mouse(mouse_move_ev(mouse_event::moved, true));
      } else {
        ref_.mouse(mouse_move_ev(mouse_event::moved, false));
      }
    }
    break;
    case WM_EXITSIZEMOVE:
    {
      RECT clirect, winrect;
      GetClientRect(
        handle_,
        &clirect);
      GetWindowRect(
        handle_,
        &winrect);

      auto cw = clirect.right - clirect.left, ch = clirect.bottom - clirect.top;
      if (w_ != cw || h_ != ch) {
        w_ = cw;
        h_ = ch;
        ref_.status(status_event{status_event::resized});
      }

      if (x_ != winrect.left || y_ != winrect.top) {
        x_ = winrect.left;
        y_ = winrect.top;
        ref_.status(status_event{status_event::moved});
      }
    }
    break;
    case WM_CLOSE:
      ref_.status(status_event{status_event::closed});
      break;
    case WM_SETCURSOR:
      ref_.cursor_.refresh();
      break;
    default: {}
  }
  return DefWindowProc(
    hwnd,
    msg,
    wparam,
    lparam);
}

} // namespace impl

void widget::close() {
  DestroyWindow(native_->handle_);
  native_->handle_ = nullptr;
}

void widget::show() {
  ShowWindow(native_->handle_, SW_SHOW);
}

void widget::hide() {
  ShowWindow(native_->handle_, SW_HIDE);
}

void widget::focus() {
  SetForegroundWindow(native_->handle_);
}

void widget::resize(uint32_t width, uint32_t height) {
  RECT winrect{0, 0, static_cast<LONG>(width), static_cast<LONG>(height)};
  AdjustWindowRect(
    &winrect,
    static_cast<DWORD>(GetWindowLongPtr(native_->handle_, GWL_STYLE)),
    false);
  SetWindowPos(
    native_->handle_,
    nullptr,
    0,
    0,
    winrect.right  - winrect.left,
    winrect.bottom - winrect.top,
    SWP_NOMOVE | SWP_NOZORDER);
}

void widget::move(int32_t x, int32_t y) {
  SetWindowPos(
    native_->handle_,
    nullptr,
    x,
    y,
    0,
    0,
    SWP_NOSIZE | SWP_NOZORDER);
}

bool widget::is_minimized() const
{ return IsIconic(native_->handle_) == TRUE; }

bool widget::is_maximized() const
{ return IsZoomed(native_->handle_) == TRUE; }

bool widget::is_visible() const
{ return IsWindowVisible(native_->handle_) == TRUE; }

bool widget::is_open() const
{ return native_->handle_ != nullptr; }

auto widget::get_handle() const -> handle*
{ return reinterpret_cast<handle*>(native_->handle_); }

void widget::set_fullscreen(bool fullscreen) {
  if (fullscreen) {
    MONITORINFO minfo;
    minfo.cbSize = sizeof(minfo);
    GetMonitorInfo(
      MonitorFromWindow(native_->handle_, MONITOR_DEFAULTTONEAREST),
      &minfo);

    SetWindowLong(
      native_->handle_,
      GWL_STYLE,
      native_->wstyle_ & ~(WS_CAPTION | WS_THICKFRAME));
    SetWindowPos(
      native_->handle_,
      nullptr,
      minfo.rcMonitor.left,
      minfo.rcMonitor.top,
      minfo.rcMonitor.right  - minfo.rcMonitor.left,
      minfo.rcMonitor.bottom - minfo.rcMonitor.top,
      SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    native_->visibility_ = widget_visibility::fullscreen;
  } else {
    SetWindowLong(
      native_->handle_,
      GWL_STYLE,
      native_->wstyle_);
    SetWindowPos(
      native_->handle_,
      nullptr,
      native_->x_,
      native_->y_,
      native_->w_,
      native_->h_,
      SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
    native_->visibility_ = widget_visibility::windowed;
  }
}

void widget::set_caption(const std::string& caption) {
  SetWindowText(native_->handle_, caption.data());
  native_->caption_ = caption;
}

void widget::set_parent(const widget* parent) {
  auto handle = parent
    ? parent->native_->handle_
    : nullptr;
  native_->wstyle_ &= handle
    ? ~WS_POPUP | WS_CHILD
    : ~WS_CHILD;

  SetWindowLong(
    native_->handle_,
    GWL_STYLE,
    native_->wstyle_);
  SetParent(native_->handle_, handle);
}

} // namespace ig
