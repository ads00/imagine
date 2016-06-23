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

#ifndef IG_ENVI_CURSOR_H
#define IG_ENVI_CURSOR_H

#include "imagine/ig.h"

#include <memory>

namespace ig   {
namespace impl { class cursor_native; class window_native; }

class window;
class IG_API cursor {
public:
  enum class shape_t {
    none, arrow, cross, hand, pointer,
    help, progress, wait, text, sizeall,
    resize_n, resize_s, resize_e, resize_w,
    resize_nw, resize_se, resize_ne, resize_sw,
    bitmap
  };
  friend impl::window_native;

  constexpr cursor(shape_t shape);
  ~cursor();

  void refresh() const;
  void reshape(shape_t shape);

  auto shape() const -> shape_t;

private:
  std::unique_ptr<impl::cursor_native> native_;
};

} // namespace ig

#endif // IG_ENVI_CURSOR_H
