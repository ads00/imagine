/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_RELATIONAL_H
#define IG_MATH_RELATIONAL_H

#include "imagine/math/theory/detail/xpr.h"

namespace ig {

template <typename Lhs, typename Rhs>
bool operator==(const xpr<Lhs>& lhs, const xpr<Rhs>& rhs) {
  assert(
    lhs.dims() ==
    rhs.dims() && "Incoherent expression comparison");

  return
    std::equal(
      lhs.begin(),
      lhs.end(),
      rhs.begin(), rhs.end());
}

template <typename Lhs, typename Rhs>
bool operator<(const xpr<Lhs>& lhs, const xpr<Rhs>& rhs) {
  assert(
    lhs.dims() ==
    rhs.dims() && "Incoherent expression comparison");

  auto pair =
    std::mismatch(
      lhs.begin(),
      lhs.end(),
      rhs.begin(), rhs.end());
  return (pair.first == lhs.end() ||
         *pair.first < *pair.second);
}

template <typename Lhs, typename Rhs>
auto operator>(const xpr<Lhs>& lhs, const xpr<Rhs>& rhs)
{ return rhs < lhs; }

template <typename Lhs, typename Rhs>
auto operator<=(const xpr<Lhs>& lhs, const xpr<Rhs>& rhs)
{ return !(lhs > rhs); }

template <typename Lhs, typename Rhs>
auto operator>=(const xpr<Lhs>& lhs, const xpr<Rhs>& rhs)
{ return !(lhs < rhs); }

template <typename Lhs, typename Rhs>
auto operator!=(const xpr<Lhs>& lhs, const xpr<Rhs>& rhs)
{ return !(lhs == rhs); }

} // namespace ig

#endif // IG_MATH_RELATIONAL_H
