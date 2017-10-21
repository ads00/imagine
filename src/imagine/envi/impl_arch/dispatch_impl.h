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

#ifndef IG_ENVI_DISPATCH_IMPL_H
#define IG_ENVI_DISPATCH_IMPL_H

#include "imagine/envi/impl_arch/config_impl.h"

#include <atomic>
#include <vector>

namespace ig   {
namespace impl {

static std::string ig_widget_class = "ig_widgetclass";

class widget_impl;
class dispatch_impl {
public:
  dispatch_impl();
  ~dispatch_impl() = default;

  std::atomic_int return_code_;
  std::atomic_bool running_;
  std::vector<widget_impl*> widgets_;

  #if defined(IG_WIN)
  static HINSTANCE reg();
  static LRESULT CALLBACK proc(
    HWND hwnd, 
    UINT msg, 
    WPARAM wparam, 
    LPARAM lparam);
  #endif
};

} // namespace impl
} // namespace ig

#endif // IG_ENVI_DISPATCH_IMPL_H
