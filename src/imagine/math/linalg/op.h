/*
 Copyright (c) 2017
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#ifndef IG_MATH_OP_H
#define IG_MATH_OP_H

#include "imagine/math/theory/matrix.h"

namespace ig  {
namespace lin {

template
< typename Lhs,
  typename Rhs >
constexpr auto dot(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs)
{ return (lhs * rhs).sum(); }

template
< typename Lhs,
  typename Rhs >
constexpr auto reflect(const matrix_base<Lhs>& i, const matrix_base<Rhs>& n)
{ return i - 2 * dot(i, n) * n; }

template
< typename Lhs,
  typename Rhs >
constexpr auto cross(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  using vector_type =
    colvec
    < std::common_type_t
      < matrix_t<Lhs>,
        matrix_t<Rhs>
      >, 3>;
  assert(
    rhs.vector() &&
    rhs.vec_size() == 3 &&
    lhs.vector() &&
    lhs.vec_size() == 3
    && "Cross exists only in three-dimensional space");

  return vector_type{lhs[1] * rhs[2] - lhs[2] * rhs[1],
                     lhs[2] * rhs[0] - lhs[0] * rhs[2],
                     lhs[0] * rhs[1] - lhs[1] * rhs[0]};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_OP_H
