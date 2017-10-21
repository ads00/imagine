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

#ifndef IG_MATH_MATRIXTRANS_H
#define IG_MATH_MATRIXTRANS_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template <typename X>
struct matrix_traits
<
  matrix_trans<X>
>
{
  using value_type = matrix_t<X>;
  static constexpr size_t n_rows = matrix_traits<X>::n_cols, n_cols = matrix_traits<X>::n_rows;
};

template <typename x_>
class matrix_trans : public matrix_base< matrix_trans<x_> > {
public:
  explicit matrix_trans(x_& xpr)
    : xpr_{xpr} {}

  auto rows() const { return xpr_.cols(); }
  auto cols() const { return xpr_.rows(); }

  decltype(auto) operator()(size_t row, size_t col) const 
  { return xpr_(col, row); }
  decltype(auto) operator()(size_t row, size_t col) 
  { return xpr_(col, row); }

  decltype(auto) operator[](size_t) const = delete;
  decltype(auto) operator[](size_t)       = delete;

private:
  x_& xpr_;
};

} // namespace ig

#endif // IG_MATH_MATRIXTRANS_H
