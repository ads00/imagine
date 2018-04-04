/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_MATRIXBLOCK_H
#define IG_MATH_MATRIXBLOCK_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template <typename X>
struct matrix_traits
<
  matrix_block<X>
>
{
  using value_type = matrix_t<X>;
  static constexpr size_t n_rows = matrix_traits<X>::n_rows, n_cols = matrix_traits<X>::n_cols;
};

template <typename x_>
class matrix_block : public matrix_base< matrix_block<x_> > {
public:
  explicit matrix_block(x_& xpr, size_t row, size_t col, size_t nr, size_t nc)
    : xpr_{xpr}
    , row_{row}
    , col_{col}
    , nr_{nr}
    , nc_{nc} {}

  auto rows() const { return nr_; }
  auto cols() const { return nc_; }

  decltype(auto) operator()(size_t row, size_t col) const
  { return xpr_(row_ + row, col_ + col); }
  decltype(auto) operator()(size_t row, size_t col)
  { return xpr_(row_ + row, col_ + col); }

  decltype(auto) operator[](size_t n) const { return xpr_((row_ + n) / nr_, (col_ + n) % nc_); }
  decltype(auto) operator[](size_t n)       { return xpr_((row_ + n) / nr_, (col_ + n) % nc_); }

  template <typename Mat>
  auto operator=(const matrix_base<Mat>& o) { return eval(*this, o); }

private:
  x_& xpr_;
  size_t row_, col_, nr_, nc_;
};

} // namespace ig

#endif // IG_MATH_MATRIXBLOCK_H
