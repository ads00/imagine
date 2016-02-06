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

#include "imagine/interact/cursor.h"
#include "imagine/interact/impl/cursor_native.h"

namespace ig
{

constexpr cursor::cursor(shape_t shape)
  : native_{std::make_unique<impl::cursor_native>(shape)}
{
}

constexpr cursor::cursor(const cursor& o)
  : native_{std::make_unique<impl::cursor_native>(*o.native_)}
{
}

cursor::~cursor() = default;

cursor::shape_t cursor::shape() const
{
  return native_->shape_;
}

cursor& cursor::operator=(const cursor& o)
{
  native_.reset(new impl::cursor_native{*o.native_});
  return *this;
}

// Native implementations
//

// void cursor::refresh() const;
// std::pair<int, int> cursor::position(const window* ref);
// void cursor::move(int x, int y, const window* ref);

} // namespace ig
