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

#include "imagine/envi/impl/mouse_impl.h"

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

auto get_x(LPARAM lparam) -> int32_t {
  return LOWORD(lparam);
}

auto get_y(LPARAM lparam) -> int32_t {
  return HIWORD(lparam);
}

auto get_wheel_delta(WPARAM wparam) -> float {
  return static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam)) / WHEEL_DELTA;
}

auto track(HWND window) -> bool {
  TRACKMOUSEEVENT tme;
  tme.cbSize = sizeof(TRACKMOUSEEVENT);
  tme.dwFlags = TME_LEAVE;
  tme.hwndTrack = window;
  return TrackMouseEvent(&tme) == TRUE;
}

} // namespace impl
} // namespace mouse
} // namespace ig
