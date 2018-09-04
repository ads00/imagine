/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_ALGEBRA_H
#define IG_MATH_ALGEBRA_H

#include "imagine/math/theory/matrix.h"

namespace ig  {
namespace lin {

template
< typename Lhs,
  typename Rhs >
constexpr auto dot(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs)
{ return (lhs * rhs).sum(); }

template <typename Mat>
constexpr auto norm(const matrix_base<Mat>& mat)
{ return std::sqrt(dot(mat, mat)); }

template <typename Mat>
constexpr auto normalise(const matrix_base<Mat>& mat)
{ return mat / norm(mat); }

template <typename Mat>
constexpr auto trace(const matrix_base<Mat>& mat)
{ return mat.diag().sum(); }

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

// null space

} // namespace lin
} // namespace ig

#endif // IG_MATH_ALGEBRA_H
