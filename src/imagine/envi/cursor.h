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

#ifndef IG_ENVI_CURSOR_H
#define IG_ENVI_CURSOR_H

#include "imagine/ig.h"

namespace ig   {
namespace impl { class cursor_impl; class widget_impl; }

enum class cursor_shape;

class widget;
class IG_API cursor {
public:
  friend impl::widget_impl;

  explicit cursor(cursor_shape shape);
  virtual ~cursor();

  void refresh() const;
  void reshape(cursor_shape shape);

  static void clip(const widget& widget);
  static void move(int32_t x, int32_t y, const widget* widget = nullptr);

private:
  std::unique_ptr<impl::cursor_impl> native_;
};

enum class cursor_shape {
  none, arrow, cross, hand, pointer,
  help, progress, wait, text, sizeall,
  resize_n, resize_s, resize_e, resize_w, resize_nw, resize_se, resize_ne, resize_sw,
  bitmap };

} // namespace ig

#endif // IG_ENVI_CURSOR_H
