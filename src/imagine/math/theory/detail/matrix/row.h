/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
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
