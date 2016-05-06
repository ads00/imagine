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

#include "imagine/envi/impl/cursor_native.h"
#include "imagine/envi/impl/window_native.h"

namespace ig   {
namespace impl {

const LPSTR cursor_tbl[] = 
{
  nullptr,         // cursor::none
  IDC_ARROW,       // cursor::arrow
  IDC_CROSS,       // cursor::cross
  IDC_HAND,        // cursor::hand
  IDC_HAND,        // cursor::pointer
  IDC_HELP,        // cursor::help
  IDC_APPSTARTING, // cursor::progress
  IDC_WAIT,        // cursor::wait
  IDC_IBEAM,       // cursor::text
  IDC_SIZEALL,     // cursor::sizeall
  IDC_SIZENS,      // cursor::resize_n
  IDC_SIZENS,      // cursor::resize_s
  IDC_SIZEWE,      // cursor::resize_e
  IDC_SIZEWE,      // cursor::resize_w
  IDC_SIZENWSE,    // cursor::resize_nw
  IDC_SIZENWSE,    // cursor::resize_se
  IDC_SIZENESW,    // cursor::resize_ne
  IDC_SIZENESW,    // cursor::resize_sw
  nullptr,         // cursor::bitmap
};

cursor_native::cursor_native(cursor::shape_t shape, int32_t x, int32_t y)
  : shape_{shape}, x_{x}, y_{y}, handle_{nullptr} {
  handle_ = static_cast<HCURSOR>(LoadImage(nullptr, cursor_tbl[shape], IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
}

cursor_native::~cursor_native() {
  DestroyCursor(handle_);
}

} // namespace impl

void cursor::refresh() const {
  SetCursor(native_->handle_);
}

void cursor::move(int32_t x, int32_t y, const window* ref) {
  auto point = POINT{x, y};
  auto res = ref ? ClientToScreen(ref->native_->handle_, &point) != 0 : true;
  if (res) {
    SetCursorPos(point.x, point.y);
  }
}

std::pair<int, int> cursor::position(const window* ref) {
  auto point = POINT{}; GetCursorPos(&point);
  auto res = ref ? ScreenToClient(ref->native_->handle_, &point) != 0 : true;
  if (res) {
    return {point.x, point.y};
  } return {};
}

} // namespace ig
