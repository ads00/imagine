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

#include "imagine/math/theory/ndarray.h"
#include "imagine/math/bridge/impl_image/bridge_jpeg.h"
#include "imagine/math/bridge/impl_image/bridge_png.h"

namespace ig {

template <> // uchar images
decltype(array2d<uint8_t>::bridge_table_) array2d<uint8_t>::bridge_table_ = {{
  {static_cast<int32_t>(ndarray_format::jpeg), std::make_tuple(impl::jpeg_validate, impl::jpeg_readp_uint8_t, impl::jpeg_write_uint8_t)},
  {static_cast<int32_t>(ndarray_format::png ), std::make_tuple( impl::png_validate,  impl::png_readp_uint8_t,  impl::png_write_uint8_t)}
  }};

template <> // uint images
decltype(array2d<uint16_t>::bridge_table_) array2d<uint16_t>::bridge_table_ = {{
  {static_cast<int32_t>(ndarray_format::png), std::make_tuple(impl::png_validate, impl::png_readp_uint16_t, impl::png_write_uint16_t)}
  }};

} // namespace ig
