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

#include "imagine/envi/impl/mouse_native.h"

namespace ig    {
namespace impl  {
namespace mouse {

button_ft buttons() {
  auto buttons = button_ft{button_t::none};
  auto swapped = GetSystemMetrics(SM_SWAPBUTTON);
  if (GetAsyncKeyState(VK_LBUTTON) < 0) {
    buttons |= swapped ? button_t::right : button_t::left;
  } if (GetAsyncKeyState(VK_RBUTTON) < 0) {
    buttons |= swapped ? button_t::left : button_t::right;
  } if (GetAsyncKeyState(VK_MBUTTON) < 0) {
    buttons |= button_t::middle;
  }
  return buttons;
}

int32_t x(LPARAM lparam) {
  return GET_X_LPARAM(lparam);
}

int32_t y(LPARAM lparam) {
  return GET_Y_LPARAM(lparam);
}

float wheel_delta(WPARAM wparam) {
  return static_cast<float>(GET_WHEEL_DELTA_WPARAM(wparam)) / WHEEL_DELTA;
}

bool track(HWND window) {
  TRACKMOUSEEVENT tme;
  tme.cbSize = sizeof(TRACKMOUSEEVENT);
  tme.dwFlags = TME_LEAVE;
  tme.hwndTrack = window;
  return TrackMouseEvent(&tme) == TRUE;
}

} // namespace mouse
} // namespace impl
} // namespace ig
