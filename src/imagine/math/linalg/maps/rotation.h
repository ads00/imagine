/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_ROTATION_H
#define IG_MATH_ROTATION_H

#include "imagine/math/linalg/op.h"

namespace ig {

template <typename Arithmetic>
class rotation {
public:
  using value_type = Arithmetic;
  static_assert(std::is_arithmetic<value_type>::value, "Plane rotation computation requires arithmetic values");

  explicit rotation(value_type c, value_type s)
    : c_{c}
    , s_{s} {}

  auto& c() const { return c_; }
  auto& s() const { return s_; }

private:
  value_type c_, s_;
};

namespace lin {

template <typename Arithmetic>
auto jacobi(Arithmetic x, Arithmetic y, Arithmetic z) {
  auto tau = (x - z) / (std::abs(y) * 2);
  auto w = std::sqrt(tau * tau + 1);
  auto t = tau > 0
    ? 1 / (tau + w)
    : 1 / (tau - w);

  auto n = 1 / std::sqrt(t * t + 1);
  return rotation<Arithmetic>{
    n,
    -sign(t) * (y / std::abs(y)) * std::abs(t) * n};
}

template <typename Arithmetic>
auto givens(Arithmetic p, Arithmetic q) {
  if (q == 0)      return std::make_pair(rotation<Arithmetic>{sign(p), Arithmetic(0)}, std::abs(p));
  else if (p == 0) return std::make_pair(rotation<Arithmetic>{Arithmetic(0), -sign(q)}, std::abs(q));
  else if (std::abs(p) > std::abs(q)) {
    auto t = q / p;
    auto u = sign(p) * std::sqrt(t * t + 1);
    auto c = 1 / u;
    return std::make_pair(
      rotation<Arithmetic>{
         c,
        -t * c},
      p * u);
  } else {
    auto t = p / q;
    auto u = sign(q) * std::sqrt(t * t + 1);
    auto s = -1 / u;
    return std::make_pair(
      rotation<Arithmetic>{
        -t * s,
         s},
      q * u);
  }
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_ROTATION_H
