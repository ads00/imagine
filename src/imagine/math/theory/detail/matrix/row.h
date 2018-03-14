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

#ifndef IG_MATH_MATRIXROW_H
#define IG_MATH_MATRIXROW_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template <typename X>
struct matrix_traits
<
  matrix_row<X>
>
{
  using value_type = matrix_t<X>;
  static constexpr size_t n_rows = 1, n_cols = matrix_traits<X>::n_cols;
};

template <typename x_>
class matrix_row : public matrix_base< matrix_row<x_> > {
public:
  explicit matrix_row(x_& xpr, size_t row)
    : xpr_{xpr}
    , row_{row} {}

  auto rows() const { return matrix_traits<matrix_row>::n_rows; }
  auto cols() const { return xpr_.cols(); }

  decltype(auto) operator[](size_t n) const
  { return xpr_(row_, n); }
  decltype(auto) operator[](size_t n)
  { return xpr_(row_, n); }

  template <typename Mat>
  auto operator=(const matrix_base<Mat>& o) { return eval(*this, o); }

private:
  x_& xpr_;
  size_t row_;
};

} // namespace ig

#endif // IG_MATH_MATRIXROW_H
