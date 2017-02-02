/*
 Copyright (c) 2017
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#ifndef IG_MATH_ITERATIVE_H
#define IG_MATH_ITERATIVE_H

#include "imagine/math/linalg/matrix.h"
#include "imagine/math/linalg/solver/precond.h"

namespace ig     {
namespace linalg {

template <typename Alg, typename Rhs, typename Lhs, typename precond>
void cg(const alg<Alg>& A, const alg<Rhs>& b, alg<Lhs>& x, const precond& precond, double tolerance = 1e-7) {
  using vector_type = precond::vector_type;

  size_t n = A.diagsize();
  vector_type r = b - A * x;

  auto threshold = tolerance * tolerance * dot(b, b);
  auto p = precond.solve(r);
  auto ro = dot(r, p), no = dot(r, r);

  vector_type z{n}, v{n};
  while (dot(r, r) > threshold) {
    v = A * p;
    auto a = ro / dot(p, v);
    x += a * p;
    r -= a * v;

    z = precond.solve(r);

    auto rn = ro;
    ro = dot(r, z), p = z + (ro / rn) * p;
  }
}

template <typename Alg, typename Rhs, typename Lhs, typename precond>
void bicgstab(const alg<Alg>& A, const alg<Rhs>& b, alg<Lhs>& x, const precond& precond, double tolerance = 1e-7) {
  using vector_type = precond::vector_type;
  using T = alg_t<Alg>;

  size_t n = A.diagsize();
  vector_type r = b - A * x;
  vector_type rn = r;

  auto threshold = tolerance * tolerance * dot(b, b);
  auto ro = dot(r, r);
  auto no = T(1), a = T(1), w = T(1);

  vector_type v{n}, p{n},
              y{n}, z{n}, s{n}, t{n};

  while (dot(r, r) > threshold) {
    auto nn = no;
    no = dot(rn, r);
    
    if (std::abs(no) < std::numeric_limits<T>::epsilon() * ro) {
      r = b - A * x;
      rn = r, no = ro = dot(r, r);
    }

    auto c = (no / nn) * (a / w);
    p = r + c * (p - w * v);

    y = precond.solve(p);
    v = A * y;

    a = no / dot(rn, v);
    s = r - a * v;

    z = precond.solve(s);
    t = A * z;

    auto tt = dot(t, t);
    w = tt > T(0)
      ? dot(t, s) / tt
      : T(0);

    x += a * y + w * z;
    r = s - w * t;
  }
}

} // namespace linalg
} // namespace ig

#endif // IG_MATH_ITERATIVE_H
