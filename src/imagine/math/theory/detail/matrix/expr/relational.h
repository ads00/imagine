/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_MATRIXRELATIONAL_H
#define IG_MATH_MATRIXRELATIONAL_H

#include "imagine/math/theory/detail/matrix/base.h"

namespace ig {

template <typename Lhs, typename Rhs>
bool operator<(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs) {
  assert(lhs.size() == rhs.size() && "Incoherent matrix comparison");

  auto pair =
    std::mismatch(
      lhs.begin(),
      lhs.end(),
      rhs.begin(), rhs.end());
  return (pair.first == lhs.end() ||
         *pair.first < *pair.second);
}

template <typename Lhs, typename Rhs>
auto operator>(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs)
{ return rhs < lhs; }

template <typename Lhs, typename Rhs>
auto operator<=(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs)
{ return !(lhs > rhs); }

template <typename Lhs, typename Rhs>
auto operator>=(const matrix_base<Lhs>& lhs, const matrix_base<Rhs>& rhs)
{ return !(lhs < rhs); }

} // namespace ig

#endif // IG_MATH_MATRIXRELATIONAL_H
