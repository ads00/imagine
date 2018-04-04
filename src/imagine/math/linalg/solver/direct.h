/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_DIRECT_H
#define IG_MATH_DIRECT_H

#include "imagine/math/theory/matrix.h"

namespace ig  {
namespace lin {

template
< typename Mat,
  typename Vec >
void forward_solve(const matrix_base<Mat>& lhs, Vec&& rhs, bool unit = false) {
  assert(
    lhs.square()
    && "Forward solver requires a square matrix");
  assert(
    rhs.vector() &&
    rhs.rows() == lhs.rows()
    && "Invalid b vector to solve");

  if (!unit) rhs[0] /= lhs(0, 0);
  for (size_t i = 1; i < lhs.cols(); ++i) {
    auto s = rhs[i];
    for (size_t j = 0; j < i; ++j)
      s -= lhs(i, j) * rhs[j];

    if (!unit) {
      rhs[i] = s / lhs(i, i);
    } else {
      rhs[i] = s;
    }
  }
}

template
< typename Mat,
  typename Vec >
void backward_solve(const matrix_base<Mat>& lhs, Vec&& rhs, bool unit = false) {
  assert(
    lhs.square()
    && "Backward solver requires a square matrix");
  assert(
    rhs.vector() &&
    rhs.rows() == lhs.rows()
    && "Invalid b vector to solve");

  if (!unit) rhs[lhs.rows() - 1] /= lhs(lhs.rows() - 1, lhs.rows() - 1);
  for (size_t i = lhs.rows() - 1; i-- > 0; ) {
    auto s = rhs[i];
    for (size_t j = i + 1; j < lhs.rows(); ++j)
      s -= lhs(i, j) * rhs[j];

    if (!unit) {
      rhs[i] = s / lhs(i, i);
    } else {
      rhs[i] = s;
    }
  }
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_DIRECT_H
