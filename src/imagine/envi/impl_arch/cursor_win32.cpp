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

#include "imagine/envi/impl_arch/cursor_impl.h"
#include "imagine/envi/impl_arch/widget_impl.h"

namespace ig   {
namespace impl {

const LPSTR cursor_tbl[] = {
  nullptr,         // cursor_shape::none
  IDC_ARROW,       // cursor_shape::arrow
  IDC_CROSS,       // cursor_shape::cross
  IDC_HAND,        // cursor_shape::hand
  IDC_HAND,        // cursor_shape::pointer
  IDC_HELP,        // cursor_shape::help
  IDC_APPSTARTING, // cursor_shape::progress
  IDC_WAIT,        // cursor_shape::wait
  IDC_IBEAM,       // cursor_shape::text
  IDC_SIZEALL,     // cursor_shape::sizeall
  IDC_SIZENS,      // cursor_shape::resize_n
  IDC_SIZENS,      // cursor_shape::resize_s
  IDC_SIZEWE,      // cursor_shape::resize_e
  IDC_SIZEWE,      // cursor_shape::resize_w
  IDC_SIZENWSE,    // cursor_shape::resize_nw
  IDC_SIZENWSE,    // cursor_shape::resize_se
  IDC_SIZENESW,    // cursor_shape::resize_ne
  IDC_SIZENESW,    // cursor_shape::resize_sw
  nullptr,         // cursor_shape::bitmap
};

cursor_impl::cursor_impl(cursor_shape shape, int32_t x, int32_t y)
  : shape_{shape}
  , x_{x}
  , y_{y}
  , handle_{nullptr} {

  handle_ = static_cast<HCURSOR>(
    LoadImage(
      nullptr, 
      cursor_tbl[static_cast<uint32_t>(shape)], 
      IMAGE_CURSOR, 
      0, 0, 
      LR_DEFAULTSIZE | LR_SHARED));
}

} // namespace impl

void cursor::refresh() const { 
  SetCursor(native_->handle_); 
}

void cursor::clip(const widget& widget) {
  RECT winrect;
  GetWindowRect(reinterpret_cast<HWND>(widget.get_handle()), &winrect);
  ClipCursor(&winrect);
}

void cursor::move(int32_t x, int32_t y, const widget* widget) {
  POINT pos{x, y};
  widget
    ? ClientToScreen(reinterpret_cast<HWND>(widget->get_handle()), &pos)
    : true;

  widget->cursor.native_->x_ = x;
  widget->cursor.native_->y_ = y;
  SetCursorPos(pos.x, pos.y);
}

} // namespace ig
