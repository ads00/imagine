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

#ifndef IG_MATH_PNG_H
#define IG_MATH_PNG_H

#include "imagine/math/theory/ndarray.h"
#include <sstream>

namespace ig     {
namespace detail {

template <typename T>
using png_t = ndarray<T, 2>;
using png_8 = ndarray<uint8_t, 2>;
using png_16 = ndarray<uint16_t, 2>;

bool png_validate(std::istream& stream);
auto png_readp_uint8_t (std::istream& stream) -> std::unique_ptr<png_8>;
auto png_readp_uint16_t(std::istream& stream) -> std::unique_ptr<png_16>;

bool png_write_uint8_t (std::ostream& stream, const png_8&  imag);
bool png_write_uint16_t(std::ostream& stream, const png_16& imag);

} // namespace detail
} // namespace ig

#endif // IG_MATH_PNG_H