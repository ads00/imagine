/*
 Imagine v0.1
 [math]
 Copyright (c) 2015-present, Hugo (hrkz) Frezat
*/

#ifndef IG_MATH_REFLECTOR_H
#define IG_MATH_REFLECTOR_H

#include "imagine/math/lin/algebra.h"

namespace ig  {

template <typename Arithmetic>
class reflector {
public:
  using value_type = Arithmetic;
  using matrix_type = matrix<value_type>;
  using vector_type = colvec<value_type>;
  static_assert(std::is_arithmetic<value_type>::value, "Elementary reflector computation requires an arithmetic vector");

  explicit reflector(
    const vector_type& ess,
    value_type tau,
    value_type beta)
    : ess_{ess}
    , tau_{tau}
    , beta_{beta} {}

  template <typename Mat> 
  void apply(matrix_base<Mat>& matrix) const;

  auto& tau() const  { return tau_; }
  auto& beta() const { return beta_; }

private:
  vector_type ess_;
  value_type  tau_, beta_;
};

template <typename Arithmetic>
template <typename Mat>
void reflector<Arithmetic>::apply(matrix_base<Mat>& matrix) const {

}

namespace lin {

template <typename Vec>
auto householder(const matrix_base<Vec>& vec) {
  auto c = vec[0];
  
  auto beta = -sign(c) * lin::norm(vec);
  auto ess = vec;
  for (size_t i = 1; i < ess.size(); ++i) ess[i] /= c - beta;

  return reflector<Vec>{
    ess, 
    (beta - c) / beta, 
    beta};
}

} // namespace lin
} // namespace ig

#endif // IG_MATH_REFLECTOR_H
