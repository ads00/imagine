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
#include "imagine/math/theory/_image/jpeg.h"
#include "imagine/math/theory/_image/png.h"

namespace ig {

template <> // uchar images
decltype(image_bridge<uint8_t, 2>::table) image_bridge<uint8_t, 2>::table = {{
  {image_format::jpeg, std::make_tuple(detail::jpeg_validate, detail::jpeg_readp_uint8_t, detail::jpeg_write_uint8_t)},
  {image_format::png , std::make_tuple( detail::png_validate,  detail::png_readp_uint8_t,  detail::png_write_uint8_t)}
  }};

template <> // uint images
decltype(image_bridge<uint16_t, 2>::table) image_bridge<uint16_t, 2>::table = {{
  {image_format::png, std::make_tuple(detail::png_validate, detail::png_readp_uint16_t, detail::png_write_uint16_t)}
  }};

} // namespace ig