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

#ifndef IG_ENVI_WINDOW_NATIVE_H
#define IG_ENVI_WINDOW_NATIVE_H

#include "imagine/envi/window.h"
#include "imagine/envi/cursor.h"
#include "imagine/envi/impl/widget_native.h"

namespace ig   {
namespace impl {

class window_native {
public:
  window_native(const window& ref);
  window_native(const window& ref, const std::string& caption, uint32_t width, uint32_t height, window::style_flags style);
  ~window_native() = default;

  const window& ref_;

  std::string caption_;
  uint32_t width_, height_;
  int32_t x_, y_;

  window::style_flags style_;
  window::visibility_t visibility_;

  bool mouse_tracked_;

  #if defined(IG_WIN)
  LRESULT internal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
  static LRESULT CALLBACK proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
  static bool reg();

  HWND handle_;
  DWORD wstyle_;
  #endif
};

} // namespace native
} // namespace ig

#endif // IG_ENVI_WINDOW_NATIVE_H
