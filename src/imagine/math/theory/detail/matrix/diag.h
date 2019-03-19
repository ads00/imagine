/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_MATRIXDIAG_H
#define IG_MATH_MATRIXDIAG_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template <typename X>
struct matrix_traits
<
  matrix_diag<X>
>
{
  using value_type = matrix_t<X>;
  static constexpr size_t n_rows = matrix_traits<X>::n_rows, n_cols = 1;
};

template <typename x_>
class matrix_diag : public matrix_base< matrix_diag<x_> > {
public:
  explicit matrix_diag(x_& xpr)
    : xpr_{xpr} {}

  auto rows() const { return xpr_.diag_size(); }
  auto cols() const { return matrix_traits<matrix_diag>::n_cols; }

  decltype(auto) operator[](size_t n) const
  { return xpr_(n, n); }
  decltype(auto) operator[](size_t n)
  { return xpr_(n, n); }

  template <typename Mat>
  auto operator=(const matrix_base<Mat>& o) { return eval(*this, o); }

private:
  x_& xpr_;
};

} // namespace ig

#endif // IG_MATH_MATRIXDIAG_H
