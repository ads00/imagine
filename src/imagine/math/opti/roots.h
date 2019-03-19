/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_ROOTS_H
#define IG_MATH_ROOTS_H

#include <imagine/math/theory/matrix.h>

namespace ig {

template 
< typename Func, 
  typename Grad, 
  typename Init>
auto newton_raphson(
  Func&& f, 
  Grad&& g, 
  Init&& x0, 
  float tol    = 1e-7f, 
  size_t iters = 1000) {

  auto xi = x0;
  auto fi = f(xi);

  size_t it = 0;
  while (lin::dot(fi, fi) > tol) {
    xi = xi - g(xi) % fi;
    fi = f(xi);
    if (it++ > iters) throw std::logic_error{"Failed to converge to an admissible root (either inexistent or too far)"};
  }
  return xi;
}

} // namespace ig

#endif // IG_MATH_ROOTS_H
