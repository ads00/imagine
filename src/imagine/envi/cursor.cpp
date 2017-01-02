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

#include "imagine/envi/arch/cursor_impl.h"
#include "imagine/envi/cursor.h"

namespace ig {

cursor::cursor(cursor_shape shape)
  : native_{std::make_unique<impl::cursor_native>(shape)} {}

cursor::~cursor() = default;

void cursor::reshape(cursor_shape shape) {
  native_.reset(new impl::cursor_native{shape, native_->x_, native_->y_});
  refresh();
}

cursor_shape cursor::get_shape() const {
  return native_->shape_;
}

// Native implementations
//

// void cursor::refresh() const;
// static void cursor::clip(const window& win);
// static void cursor::move(int32_t x, int32_t y, const window* win = nullptr);

} // namespace ig
