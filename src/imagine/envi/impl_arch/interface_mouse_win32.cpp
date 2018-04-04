/*
 Imagine v0.1
 [envi]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#include "imagine/envi/impl_arch/interface_mouse_impl.h"

namespace ig    {
namespace mouse {
namespace impl  {

auto get_buttons() -> buttons {
  auto swap = GetSystemMetrics(SM_SWAPBUTTON);

  buttons buttons{button::none};
  if (GetAsyncKeyState(VK_LBUTTON) < 0)
    buttons |= swap
      ? button::right
      : button::left;
  if (GetAsyncKeyState(VK_RBUTTON) < 0)
    buttons |= swap
      ? button::left
      : button::right;
  if (GetAsyncKeyState(VK_MBUTTON) < 0)
    buttons |= button::middle;
  return buttons;
}

auto get_x(LPARAM lparam) -> int32_t
{ return LOWORD(lparam); }

auto get_y(LPARAM lparam) -> int32_t
{ return HIWORD(lparam); }

auto get_wheel_delta(WPARAM wparam) -> float
{ return static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam)) / WHEEL_DELTA; }

auto track(HWND widget) -> bool {
  TRACKMOUSEEVENT tme;
  tme.cbSize = sizeof(TRACKMOUSEEVENT);
  tme.dwFlags = TME_LEAVE;
  tme.hwndTrack = widget;
  return TrackMouseEvent(&tme) == TRUE;
}

} // namespace impl
} // namespace mouse
} // namespace ig
