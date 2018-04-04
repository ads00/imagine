/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_ITERATIVE_H
#define IG_MATH_ITERATIVE_H

#include "imagine/math/linalg/solver/precondition.h"

namespace ig  {
namespace lin {

template
< typename Mat,
  typename Rhs,
  typename Lhs,
  typename Precond >
void cg(const matrix_base<Mat>& A, const matrix_base<Rhs>& b, matrix_base<Lhs>& x, const Precond& pre, double tolerance = 1e-7) {
  using vector_type = typename Precond::vector_type;

  auto n = A.diagsize();
  vector_type r = b - A % x;

  auto threshold = tolerance * tolerance * dot(b, b);
  auto p = pre.solve(r);
  auto ro = dot(r, p), no = dot(r, r);

  vector_type z{n}, v{n};
  while (dot(r, r) > threshold) {
    v = A % p;
    auto a = ro / dot(p, v);
    x += a * p;
    r -= a * v;

    z = pre.solve(r);

    auto rn = ro;
    ro = dot(r, z), p = z + (ro / rn) * p;
  }
}

template
< typename Mat,
  typename Rhs,
  typename Lhs,
  typename Precond >
void bicgstab(const matrix_base<Mat>& A, const matrix_base<Rhs>& b, matrix_base<Lhs>& x, const Precond& pre, double tolerance = 1e-7) {
  using value_type = typename Precond::value_type;
  using vector_type = typename Precond::vector_type;

  auto n = A.diagsize();
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
      rn = r, no = ro = dot(r, r);
    }

    auto c = (no / nn) * (a / w);
    p = r + c * (p - w * v);

    y = pre.solve(p); v = A % y;
    a = no / dot(rn, v);
    s = r - a * v;
    z = pre.solve(s); t = A % z;

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

#endif // IG_MATH_ITERATIVE_H
