/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_BICGSTAB_H
#define IG_MATH_BICGSTAB_H

#include "imagine/math/lin/algebra.h"

namespace ig  {
namespace lin {

template
< typename Mat,
  typename Rhs,
  typename Lhs,
  typename Precond >
void bicgstab(const matrix_base<Mat>& A, const matrix_base<Rhs>& b, matrix_base<Lhs>& x, Precond&& c, float tolerance = 1e-7) {
  using value_type = typename Precond::value_type;
  using vector_type = typename Precond::vector_type;

  auto n = A.diag_size();
  vector_type r = b - A % x;
  vector_type rn = r;

  auto threshold = tolerance * tolerance * dot(b, b);
  auto ro = dot(r, r);
  value_type no = 1, a = 1, w = 1;

  vector_type v{n}, p{n},
              y{n}, z{n}, s{n}, t{n};

  while (dot(r, r) > threshold) {
    auto nn = no;
    no = dot(rn, r);

    if (std::abs(no) < std::numeric_limits<value_type>::epsilon() * ro) {
      r = b - A % x;
      rn = r;
      no = ro = dot(r, r);
    }

    auto q = (no / nn) * (a / w);
    p = r + q * (p - w * v);

    y = c.step(p); 
    v = A % y;
    a = no / dot(rn, v);
    s = r - a * v;
    z = c.step(s); 
    t = A % z;

    auto tt = dot(t, t);
    w = tt > 0
      ? dot(t, s) / tt
      : 0;

    x += a * y + w * z;
    r  = s - w * t;
  }
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_BICGSTAB_H
