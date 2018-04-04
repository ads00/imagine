/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_MATRIXCOL_H
#define IG_MATH_MATRIXCOL_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template <typename X>
struct matrix_traits
<
  matrix_col<X>
>
{
  using value_type = matrix_t<X>;
  static constexpr size_t n_rows = matrix_traits<X>::n_rows, n_cols = 1;
};

template <typename x_>
class matrix_col : public matrix_base< matrix_col<x_> > {
public:
  explicit matrix_col(x_& xpr, size_t col)
    : xpr_{xpr}
    , col_{col} {}

  auto rows() const { return xpr_.rows(); }
  auto cols() const { return matrix_traits<matrix_col>::n_cols; }

  decltype(auto) operator[](size_t n) const
  { return xpr_(n, col_); }
  decltype(auto) operator[](size_t n)
  { return xpr_(n, col_); }

  template <typename Mat>
  auto operator=(const matrix_base<Mat>& o) { return eval(*this, o); }

private:
  x_& xpr_;
  size_t col_;
};

} // namespace ig

#endif // IG_MATH_MATRIXCOL_H
