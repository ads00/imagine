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

#include "imagine/math/theory/data.h"
#include "imagine/math/bridge/impl_image/bridge_jpeg.h"
#include "imagine/math/bridge/impl_image/bridge_png.h"

namespace ig {

template <>
decltype(data8_t::bridge_table_) data8_t::bridge_table_ = {{
  {format_t::jpeg, std::make_tuple(impl::jpeg_validate, impl::jpeg_read_8, impl::jpeg_write_8)},
  {format_t::png,  std::make_tuple(impl::png_validate,  impl::png_read_8,  impl::png_write_8)}
}};

template <>
decltype(data16_t::bridge_table_) data16_t::bridge_table_ = {{
  {format_t::png, std::make_tuple(impl::png_validate, impl::png_read_16, impl::png_write_16)}
}};

} // namespace ig
