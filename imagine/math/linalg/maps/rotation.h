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

#ifndef IG_MATH_ROTATION_H
#define IG_MATH_ROTATION_H

#include "imagine/math/theory/matrix.h"

namespace ig {

template <typename T>
class rotation {
public:
  using type = T;
  static_assert(std::is_arithmetic<type>::value, "Plane rotation computation requires arithmetic values");

  explicit rotation(type c, type s)
    : c_{c}
    , s_{s} {}

  template <typename Lhs, typename Rhs>
  void apply(matrix_base<Lhs>& x, matrix_base<Rhs>& y) {
    assert(x.size() == y.size() && "Incoherent plane vectors");
    assert(x.vector() && y.vector() && "Plane rotation can only be applied on vectors");

    for (size_t i = 0; i < x.size(); ++i) {
      auto xi = x[i], 
           yi = y[i];
      x[i] =  c_ * xi + s_ * yi; 
      y[i] = -s_ * xi + c_ * yi;
    }
  }

  auto& c() const { return c_; }
  auto& s() const { return s_; }

private:
  T c_, s_;
};

template <typename T>
constexpr auto operator*(const rotation<T>& lhs, const rotation<T>& rhs) {
  return rotation<T>{lhs.c_ * rhs.c_ - lhs.s_ * rhs.s_,
                     lhs.c_ * rhs.s_ + lhs.s_ * rhs.c_};
}

namespace lin {

template <typename T>
auto jacobi(T x, T y, T z) {
  auto tau = (x - z) / (std::abs(y) * 2);
  auto w = std::sqrt(tau * tau + 1);
  auto t = tau > 0
    ? 1 / (tau + w)
    : 1 / (tau - w);

  auto n = 1 / std::sqrt(t * t + 1);
  return rotation<T>{n, -sign(t) * (y / std::abs(y)) * std::abs(t) * n};
}

template <typename T>
auto givens(T p, T q) {
  if      (q == 0) return std::make_pair(rotation<T>{sign(p),  T(0)}, std::abs(p));
  else if (p == 0) return std::make_pair(rotation<T>{T(0), -sign(q)}, std::abs(q));
  else if (std::abs(p) > std::abs(q)) {
    auto t = q / p;
    auto u = sign(p) * std::sqrt(t * t + 1);
    auto c = 1 / u;
    return std::make_pair(rotation<T>{c, -t * c}, p * u);
  } else {
    auto t = p / q;
    auto u = sign(q) * std::sqrt(t * t + 1);
    auto s = -1 / u;
    return std::make_pair(rotation<T>{-t * s, s}, q * u);
  }
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_ROTATION_H
