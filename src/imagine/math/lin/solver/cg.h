/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_CG_H
#define IG_MATH_CG_H

#include "imagine/math/lin/algebra.h"

namespace ig  {
namespace lin {

template
< typename Mat,
  typename Rhs,
  typename Lhs,
  typename Precond >
void cg(const matrix_base<Mat>& A, const matrix_base<Rhs>& b, matrix_base<Lhs>& x, Precond&& c, float tolerance = 1e-7) {
  using vector_type = typename Precond::vector_type;

  auto n = A.diag_size();
  vector_type r = b - A % x;

  auto threshold = tolerance * tolerance * dot(b, b);
  auto p = c.step(r);
  auto ro = dot(r, p), no = dot(r, r);

  vector_type z{n}, v{n};
  while (dot(r, r) > threshold) {
    v = A % p;
    auto a = ro / dot(p, v);
    x += a * p;
    r -= a * v;

    z = c.step(r);

    auto rn = ro;
    ro = dot(r, z);
    p = z + (ro / rn) * p;
  }
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_CG_H
